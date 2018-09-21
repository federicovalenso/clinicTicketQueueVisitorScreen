#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "ticketprocessor.h"
#include "appsettings.h"

TicketProcessor::TicketProcessor(QObject *parent) :
    QObject(parent)
{
    mNetworkManager = new QNetworkAccessManager(this);
    connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

void TicketProcessor::sendGetTicketRequest() const noexcept
{
    sendGetRequest(ACTION_TICKETS_FREE);
}

void TicketProcessor::sendGetRequest(const QString& action) const noexcept
{
    AppSettings& settings = AppSettings::getInstance();
    QString address = QString("http://%1:%2/%3")
            .arg(settings.getServerAddr())
            .arg(settings.getPort())
            .arg(action);
    QUrl url(std::move(address));
    QNetworkRequest request(url);
    mNetworkManager->get(request);
}

void TicketProcessor::sendPutRequest(const QUrlQuery &params, const QString &action) const noexcept
{
    AppSettings& settings = AppSettings::getInstance();
    QString address = QString("http://%1:%2/%3")
            .arg(settings.getServerAddr())
            .arg(settings.getPort())
            .arg(action);
    QUrl url(address);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    mNetworkManager->put(request, params.toString(QUrl::FullyEncoded).toUtf8());
}

void TicketProcessor::sendVoiceTicketRequest(const Ticket& ticket) const noexcept
{
    QUrlQuery params;
    params.addQueryItem(ID, QString::number(ticket.id));
    params.addQueryItem(IS_VOICED, QString::number(1));
    sendPutRequest(params, ACTION_VOICE_TICKETS);
}

Ticket TicketProcessor::parseTicket(const QByteArray &data)
{
    QJsonDocument jsonDoc(QJsonDocument::fromJson(data));
    Ticket result;
    if (jsonDoc.isObject()) {
        QJsonObject jsonObj(jsonDoc.object());
        if (jsonObj.contains(ID) && jsonObj.contains(TICKET_NUMBER) && jsonObj.contains(WINDOW_NUMBER)) {
            result.id = jsonObj.value(ID).toInt();
            result.ticket_number = jsonObj.value(TICKET_NUMBER).toString();
            result.window = jsonObj.value(WINDOW_NUMBER).toInt();
        }
    }
    return result;
}

void TicketProcessor::replyFinished(QNetworkReply *reply)
{
    auto error = reply->error();
    if (error != QNetworkReply::NetworkError::NoError) {
        emit requestError(tr("Не удалось получить данные с сервера..."));
        qWarning() << reply->error();
    } else {
        int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (status == 200) {
            QNetworkAccessManager::Operation operation = reply->operation();
            if (operation == QNetworkAccessManager::GetOperation) {
                Ticket ticket(parseTicket(reply->readAll()));
                if (ticket.isValid()) {
                    emit receivedTicket(ticket);
                    sendVoiceTicketRequest(ticket);
                } else {
                    emit requestError(tr("Отсутствуют неозвученные талоны"));
                }
            }
        }
    }
}
