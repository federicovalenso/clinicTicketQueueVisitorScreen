#ifndef PLAYER_H
#define PLAYER_H

#include <queue>
#include <vector>
#include <string>
#include <QString>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "ticket.h"

class TicketsPlayer : QObject
{
    Q_OBJECT
public:
    TicketsPlayer(QObject* parent = nullptr);
    void addTicketToPlaylist(const Ticket& ticket);

private slots:
    void tryToPlay();

private:
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    std::queue<Ticket> tickets;
    const QString soundResPrefix = "qrc:/lettersAndNumbers/";

    std::vector<QString> parseTicketNumber(const QString &ticket);
};

#endif // PLAYER_H
