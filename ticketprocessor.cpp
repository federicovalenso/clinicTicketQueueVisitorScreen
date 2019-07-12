#include "ticketprocessor.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkCookieJar>
#include "appsettings.h"

const QString TicketProcessor::ACTION_TICKETS = "tickets";
const QString TicketProcessor::ACTION_VOICE_TICKETS = "api/tickets";
const QString TicketProcessor::ACTION_TICKETS_FREE = "tickets/free";
const QString TicketProcessor::ACTION_LOGIN = "login";
const QString TicketProcessor::ACTION_LOGIN_PATH =
    "/" + TicketProcessor::ACTION_LOGIN;
const QString TicketProcessor::ID = "id";
const QString TicketProcessor::TICKET_NUMBER = "ticket_number";
const QString TicketProcessor::WINDOW_NUMBER = "window_number";
const QString TicketProcessor::IS_VOICED = "is_voiced";
const QString TicketProcessor::NAME_PARAM = "name";
const QString TicketProcessor::PASSWORD_PARAM = "password";
const QByteArray TicketProcessor::SET_COOKIE_HEADER = "Set-Cookie";
const QByteArray TicketProcessor::SESSION_ID = "sessionid";

TicketProcessor::TicketProcessor(QObject *parent) : QObject(parent) {
  mNetworkManager = new QNetworkAccessManager(this);
  connect(mNetworkManager, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(replyFinished(QNetworkReply *)));
}

void TicketProcessor::sendLoginRequest(const QString &name,
                                       const QString &password) const noexcept {
  QUrlQuery params;
  params.addQueryItem(NAME_PARAM, name);
  params.addQueryItem(PASSWORD_PARAM, password);
  sendPostRequest(params, ACTION_LOGIN);
}

void TicketProcessor::sendGetTicketRequest() const noexcept {
  sendGetRequest(ACTION_TICKETS_FREE);
}

void TicketProcessor::login() {
  const auto &settings = AppSettings::getInstance();
  sendLoginRequest(settings.getUserName(), settings.getPassword());
  qInfo() << "Sending login request...";
}

void TicketProcessor::sendGetRequest(const QString &action) const noexcept {
  AppSettings &settings = AppSettings::getInstance();
  QString address = QString("http://%1:%2/%3")
                        .arg(settings.getServerName())
                        .arg(settings.getPort())
                        .arg(action);
  QUrl url(std::move(address));
  QNetworkRequest request(url);
  mNetworkManager->get(request);
}

void TicketProcessor::sendPostRequest(const QUrlQuery &params,
                                      const QString &action) const noexcept {
  AppSettings &settings = AppSettings::getInstance();
  QString address = QString("http://%1:%2/%3")
                        .arg(settings.getServerName())
                        .arg(settings.getPort())
                        .arg(action);
  QUrl url(address);
  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "application/x-www-form-urlencoded");
  mNetworkManager->post(request, params.toString(QUrl::FullyEncoded).toUtf8());
}

void TicketProcessor::sendPutRequest(const QUrlQuery &params,
                                     const QString &action) const noexcept {
  AppSettings &settings = AppSettings::getInstance();
  QString address = QString("http://%1:%2/%3")
                        .arg(settings.getServerName())
                        .arg(settings.getPort())
                        .arg(action);
  QUrl url(address);
  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader,
                    "application/x-www-form-urlencoded");
  mNetworkManager->put(request, params.toString(QUrl::FullyEncoded).toUtf8());
}

void TicketProcessor::sendVoiceTicketRequest(const Ticket &ticket) const
    noexcept {
  QUrlQuery params;
  params.addQueryItem(ID, QString::number(ticket.id));
  params.addQueryItem(IS_VOICED, QString::number(1));
  sendPutRequest(params, ACTION_VOICE_TICKETS);
}

Ticket TicketProcessor::parseTicket(const QByteArray &data) {
  QJsonDocument jsonDoc(QJsonDocument::fromJson(data));
  Ticket result;
  if (jsonDoc.isObject()) {
    QJsonObject jsonObj(jsonDoc.object());
    if (jsonObj.contains(ID) && jsonObj.contains(TICKET_NUMBER) &&
        jsonObj.contains(WINDOW_NUMBER)) {
      result.id = jsonObj.value(ID).toInt();
      result.ticket_number = jsonObj.value(TICKET_NUMBER).toString();
      result.window = jsonObj.value(WINDOW_NUMBER).toInt();
    }
  }
  return result;
}

QNetworkCookie TicketProcessor::getCookie(const QNetworkReply &reply) {
  QNetworkCookie result;
  if (reply.hasRawHeader(SET_COOKIE_HEADER)) {
    auto cookie_header = reply.rawHeader(SET_COOKIE_HEADER);
    auto cookie_parts = cookie_header.split(';');
    for (const auto &part : cookie_parts) {
      auto params = part.split('=');
      if (params.size() == 2) {
        if (params.first() == SESSION_ID) {
          result.setName(params.first());
          result.setValue(params.last());
        }
      }
    }
  }
  return result;
}

void TicketProcessor::replyFinished(QNetworkReply *reply) {
  auto error = reply->error();
  qDebug() << reply->url();
  if (error != QNetworkReply::NetworkError::NoError) {
    qWarning() << reply->error();
    if (error == QNetworkReply::NetworkError::ContentAccessDenied) {
      emit needLogin();
    }
  } else {
    int status =
        reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
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
      } else if (operation == QNetworkAccessManager::PostOperation) {
        if (reply->url().path() == ACTION_LOGIN_PATH) {
          auto cookie = getCookie(*reply);
          if (!cookie.value().isEmpty()) {
            reply->manager()->cookieJar()->insertCookie(cookie);
          } else {
            emit requestError(tr(
                "Авторизация успешна, однако, сервер не вернул данных сессии"));
          }
          qInfo() << "Login's succesfull";
          emit(loginSuccess());
        }
      }
    }
  }
}
