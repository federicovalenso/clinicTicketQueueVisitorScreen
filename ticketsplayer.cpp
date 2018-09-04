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
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(tryToPlay()));
}

void TicketsPlayer::addTicketToPlaylist(const Ticket &ticket)
{
    tickets.push(ticket);
    tryToPlay();
}

void TicketsPlayer::tryToPlay()
{
    if (tickets.size() > 0) {
        if (player->state() != QMediaPlayer::State::PlayingState) {
            playlist->clear();
            playlist->addMedia(QUrl(soundResPrefix + "ticket"));
            for (const auto& token : parseTicketNumber(tickets.front().ticket_number)) {
                playlist->addMedia(QUrl((soundResPrefix + token)));
            }
            playlist->setCurrentIndex(0);
            player->play();
            tickets.pop();
        }
    }
}

vector<QString> TicketsPlayer::parseTicketNumber(const QString& ticket)
{
    auto digitPos = ticket.indexOf(QRegExp("[1-9]"));
    vector<QString> result;
    result.push_back(ticket.left(digitPos-1));
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
    return result;
}
