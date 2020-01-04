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
  static const QString kActionTickets;
  static const QString kActionVoiceTickets;
  static const QString kActionFreeTickets;
  static const QString kActionLogin;
  static const QString kActionLoginPath;
  static const QString kId;
  static const QString kTicketNumber;
  static const QString kWindow;
  static const QString kIsVoiced;
  static const QString kNameParam;
  static const QString kPasswordParam;
  static const QByteArray kSetCookieHeader;
  static const QByteArray kSessionId;

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
  std::optional<Ticket> parseTicket(const QByteArray&);
};

#endif  // TICKETPROCESSOR_H
