#include "ticketsplayer.h"

#include <QRegExp>
#include <QDebug>

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
            [=](QMediaPlayer::State newState)
            {
                if (newState == QMediaPlayer::State::StoppedState) {
                    this->playlist->clear();
                    qDebug() << "playlist's been cleared";
                }
            });
}

void TicketsPlayer::addTicketToPlaylist(const Ticket &ticket)
{
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

void TicketsPlayer::tryToPlay()
{
    if (tickets.size() > 0) {
        playlist->addMedia(QUrl(SOUND_RES_PREFIX + TICKET_FILE_NAME));
        Ticket ticket = tickets.back();
        for (const auto& token : parseTicketNumber(ticket.ticket_number)) {
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

vector<QString> TicketsPlayer::parseTicketNumber(const QString& ticket)
{
    auto digitPos = ticket.indexOf(QRegExp("[1-9]"));
    vector<QString> result;
    QString transLetter = lettersTrans.value(ticket.left(digitPos).toLower());
    if (transLetter.isEmpty() == false) {
        result.push_back(std::move(transLetter));
        int number = ticket.right(digitPos).toInt();
        int hundreds = number / 100;
        if (hundreds > 0) {
            result.push_back(QString::number(hundreds * 100));
        }
        int minusHundreds = number - hundreds * 100;
        if (minusHundreds > 10 && minusHundreds < 20) {
            result.push_back(QString::number(minusHundreds));
        } else {
            int tens = minusHundreds / 10;
            int ones = number % 10;
            if (tens > 0) {
                result.push_back(QString::number(tens * 10));
            }
            if (ones > 0) {
                result.push_back(QString::number(ones));
            }
        }
    }
    return result;
}
