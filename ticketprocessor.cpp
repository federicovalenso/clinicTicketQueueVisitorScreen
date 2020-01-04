#include "ticketprocessor.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkCookieJar>
#include "appsettings.h"

const QString TicketProcessor::kActionTickets = "tickets";
const QString TicketProcessor::kActionVoiceTickets = "api/ticket/voice";
const QString TicketProcessor::kActionFreeTickets = "api/tickets/free";
const QString TicketProcessor::kActionLogin = "login";
const QString TicketProcessor::kActionLoginPath =
    "/" + TicketProcessor::kActionLogin;
const QString TicketProcessor::kId = "id";
const QString TicketProcessor::kTicketNumber = "ticket_number";
const QString TicketProcessor::kWindow = "window";
const QString TicketProcessor::kIsVoiced = "is_voiced";
const QString TicketProcessor::kNameParam = "name";
const QString TicketProcessor::kPasswordParam = "password";
const QByteArray TicketProcessor::kSetCookieHeader = "Set-Cookie";
const QByteArray TicketProcessor::kSessionId = "sessionid";

TicketProcessor::TicketProcessor(QObject *parent) : QObject(parent) {
  mNetworkManager = new QNetworkAccessManager(this);
  connect(mNetworkManager, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(replyFinished(QNetworkReply *)));
}

void TicketProcessor::sendLoginRequest(const QString &name,
                                       const QString &password) const noexcept {
  QUrlQuery params;
  params.addQueryItem(kNameParam, name);
  params.addQueryItem(kPasswordParam, password);
  sendPostRequest(params, kActionLogin);
}

void TicketProcessor::sendGetTicketRequest() const noexcept {
  sendGetRequest(kActionFreeTickets);
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
  params.addQueryItem(kId, QString::number(ticket.id));
  params.addQueryItem(kIsVoiced, QString::number(1));
  sendPutRequest(params, kActionVoiceTickets);
}

std::optional<Ticket> TicketProcessor::parseTicket(const QByteArray &data) {
  QJsonDocument jsonDoc(QJsonDocument::fromJson(data));
  Ticket result;
  if (jsonDoc.isObject()) {
    QJsonObject jsonObj(jsonDoc.object());
    if (jsonObj.contains(kId) && jsonObj.contains(kTicketNumber) &&
        jsonObj.contains(kWindow)) {
      result.id = jsonObj.value(kId).toInt();
      result.ticket_number = jsonObj.value(kTicketNumber).toString();
      result.window = jsonObj.value(kWindow).toInt();
      return result;
    }
  }
  return std::nullopt;
}

QNetworkCookie TicketProcessor::getCookie(const QNetworkReply &reply) {
  QNetworkCookie result;
  if (reply.hasRawHeader(kSetCookieHeader)) {
    auto cookie_header = reply.rawHeader(kSetCookieHeader);
    auto cookie_parts = cookie_header.split(';');
    for (const auto &part : cookie_parts) {
      auto params = part.split('=');
      if (params.size() == 2) {
        if (params.first() == kSessionId) {
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
        const auto ticket(parseTicket(reply->readAll()));
        if (ticket) {
          emit receivedTicket(*ticket);
          sendVoiceTicketRequest(*ticket);
        } else {
          emit requestError(tr("Отсутствуют неозвученные талоны"));
        }
      } else if (operation == QNetworkAccessManager::PostOperation) {
        if (reply->url().path() == kActionLoginPath) {
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
