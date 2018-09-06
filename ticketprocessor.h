#ifndef TICKETPROCESSOR_H
#define TICKETPROCESSOR_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "ticket.h"

class TicketProcessor : public QObject
{
    Q_OBJECT
public:
    explicit TicketProcessor(QObject *parent = nullptr);
    void sendGetTicketRequest() const noexcept;

signals:
    void requestError(const QString& message);
    void receivedTicket(const Ticket& ticket);

private slots:
    void replyFinished(QNetworkReply *reply);

private:
    const QString ACTION_TICKETS = "tickets";
    const QString ACTION_VOICE_TICKETS = "api/tickets";
    const QString ACTION_TICKETS_FREE = "tickets/free";
    const QString ID = "id";
    const QString TICKET_NUMBER = "ticket_number";
    const QString WINDOW_NUMBER = "window_number";
    const QString IS_VOICED = "is_voiced";
    QNetworkAccessManager *mNetworkManager;
    void sendGetRequest(const QString &action) const noexcept;
    void sendPutRequest(const QUrlQuery& params, const QString& action) const noexcept;
    void sendVoiceTicketRequest(const Ticket& ticket) const noexcept;
    Ticket parseTicket(const QByteArray&);
};

#endif // TICKETPROCESSOR_H
