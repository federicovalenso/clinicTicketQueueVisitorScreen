#ifndef PLAYER_H
#define PLAYER_H

#include <queue>
#include <vector>
#include <QMap>
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
    const QString SOUND_RES_PREFIX = "qrc:/lettersAndNumbers/";
    const QString TICKET_FILE_NAME = "ticket";
    const QString WINDOW_FILE_NAME = "window";
    const size_t MAX_TICKETS = 4;
    const QMap<QString, QString> lettersTrans = {
        {"б", "b"},
        {"в", "v"},
        {"д", "d"},
        {"п", "p"},
        {"р", "r"},
        {"с", "s"},
        {"э", "e"}
    };

    std::vector<QString> parseTicketNumber(const QString &ticket);
};

#endif // PLAYER_H
