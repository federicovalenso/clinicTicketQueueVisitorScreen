#ifndef PLAYER_H
#define PLAYER_H

#include <queue>
#include <vector>
#include <QString>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMutex>
#include "ticket.h"

class TicketsPlayer : public QObject
{
    Q_OBJECT
public:
    TicketsPlayer(QObject* parent = nullptr);
    void addTicketToPlaylist(const Ticket& ticket);
    QMediaPlayer::State state() const;

private slots:
    void tryToPlay();

private:
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    std::queue<Ticket> tickets;
    QMutex mutex;
    const QString SOUND_RES_PREFIX = "qrc:/lettersAndNumbers/";
    const QString TICKET_FILE_NAME = "ticket";
    const QString WINDOW_FILE_NAME = "window";
    const size_t MAX_TICKETS = 4;
};

#endif // PLAYER_H
