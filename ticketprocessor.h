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
    static const QString ACTION_TICKETS_FREE;
    const QString ID = "id";
    const QString TICKET_NUMBER = "ticket_number";
    const QString WINDOW_NUMBER = "window_number";
    void sendGetRequest(const QString &action) const noexcept;
    QNetworkAccessManager *mNetworkManager;
    Ticket parseTicket(const QByteArray&);
};

#endif // TICKETPROCESSOR_H
