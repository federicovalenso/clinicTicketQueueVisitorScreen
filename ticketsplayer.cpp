#include <QMutexLocker>
#include <QDebug>
#include "ticketsplayer.h"
#include "ticketnumberparser.h"

using namespace std;

TicketsPlayer::TicketsPlayer(QObject *parent)
    : QObject (parent)
{
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);
    player->setPlaylist(playlist);
    connect(player,
            &QMediaPlayer::stateChanged,
            this,
            [=] (QMediaPlayer::State newState)
            {
                if (newState == QMediaPlayer::State::StoppedState) {
                    QMutexLocker locker(&mutex);
                    playlist->clear();
                    qDebug() << "playlist's been cleared";
                }
            });
}

void TicketsPlayer::addTicketToPlaylist(const Ticket &ticket)
{
    QMutexLocker locker(&mutex);
    if (tickets.size() > 0) {
        if (tickets.front() != ticket) {
            tickets.push(ticket);
            tryToPlay();
        }
    } else {
        tickets.push(ticket);
        tryToPlay();
    }
}

QMediaPlayer::State TicketsPlayer::state() const
{
    return player->state();
}

void TicketsPlayer::tryToPlay()
{
    if (tickets.size() > 0) {
        playlist->addMedia(QUrl(SOUND_RES_PREFIX + TICKET_FILE_NAME));
        Ticket ticket = tickets.back();
        for (const auto& token : TicketNumberParser::parse(ticket.ticket_number)) {
            playlist->addMedia(QUrl(SOUND_RES_PREFIX + token));
        }
        playlist->addMedia(QUrl(SOUND_RES_PREFIX + WINDOW_FILE_NAME));
        playlist->addMedia(QUrl(SOUND_RES_PREFIX + QString::number(ticket.window)));
        if (player->state() != QMediaPlayer::State::PlayingState) {
            player->play();
        }
        if (tickets.size() == MAX_TICKETS) {
            tickets.pop();
        }
    }
}
