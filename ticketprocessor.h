#ifndef TICKETPROCESSOR_H
#define TICKETPROCESSOR_H

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QUrlQuery>
#include "ticket.h"

class TicketProcessor : public QObject {
  Q_OBJECT
 public:
  explicit TicketProcessor(QObject* parent = nullptr);
  void sendGetTicketRequest() const noexcept;

 signals:
  void requestError(const QString& message);
  void receivedTicket(const Ticket& ticket);
  void needLogin(int ms = 2000);
  void loginSuccess();

 public slots:
  void login();

 private slots:
  void replyFinished(QNetworkReply* reply);

 private:
  static const QString ACTION_TICKETS;
  static const QString ACTION_VOICE_TICKETS;
  static const QString ACTION_TICKETS_FREE;
  static const QString ACTION_LOGIN;
  static const QString ACTION_LOGIN_PATH;
  static const QString ID;
  static const QString TICKET_NUMBER;
  static const QString WINDOW_NUMBER;
  static const QString IS_VOICED;
  static const QString NAME_PARAM;
  static const QString PASSWORD_PARAM;
  static const QByteArray SET_COOKIE_HEADER;
  static const QByteArray SESSION_ID;

  QNetworkAccessManager* mNetworkManager;

  void sendGetRequest(const QString& action) const noexcept;
  void sendPostRequest(const QUrlQuery& params, const QString& action) const
      noexcept;
  void sendPutRequest(const QUrlQuery& params, const QString& action) const
      noexcept;
  void sendLoginRequest(const QString& name, const QString& password) const
      noexcept;
  void sendVoiceTicketRequest(const Ticket& ticket) const noexcept;
  QNetworkCookie getCookie(const QNetworkReply& reply);
  Ticket parseTicket(const QByteArray&);
};

#endif  // TICKETPROCESSOR_H
