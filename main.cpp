#include <QGuiApplication>
#include <QMediaPlayer>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

#ifdef Q_OS_ANDROID
#include <QAndroidJniEnvironment>
#include <QAndroidJniObject>
#include <QtAndroid>
#endif

#include "appsettings.h"
#include "ticket.h"
#include "ticketmodel.h"
#include "ticketprocessor.h"
#include "ticketsplayer.h"

#ifdef Q_OS_ANDROID
void keep_screen_on(bool on) {
  QtAndroid::runOnAndroidThread([on] {
    QAndroidJniObject activity = QtAndroid::androidActivity();
    if (activity.isValid()) {
      QAndroidJniObject window =
          activity.callObjectMethod("getWindow", "()Landroid/view/Window;");

      if (window.isValid()) {
        const int FLAG_KEEP_SCREEN_ON = 128;
        if (on) {
          window.callMethod<void>("addFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
        } else {
          window.callMethod<void>("clearFlags", "(I)V", FLAG_KEEP_SCREEN_ON);
        }
      }
    }
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
      env->ExceptionClear();
    }
  });
}
#endif

using namespace std;

int main(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);
  QGuiApplication::setOrganizationName(R"(MANO 'MDC')");
  QGuiApplication::setApplicationName(R"(Mano 'MDC' Ticket's scoreboard)");

#ifdef Q_OS_ANDROID
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  keep_screen_on(true);
#endif

  auto& settings = AppSettings::getInstance();
  auto* model = new TicketModel(&app, settings.getMaxRows());
  QObject::connect(&settings, SIGNAL(maxRowSettingChanged(int)), model,
                   SLOT(changeMaxTicket(int)));
  QQmlApplicationEngine engine;
  engine.rootContext()->setContextProperty("ticketsModel", model);
  engine.rootContext()->setContextProperty("appSettings", &settings);

  engine.load(QUrl(QStringLiteral("qrc:/views/ticketsView.qml")));
  if (engine.rootObjects().isEmpty()) {
    return -1;
  }

  auto* player = new TicketsPlayer(&app);
  auto* processor = new TicketProcessor(&app);
  QObject::connect(processor, &TicketProcessor::receivedTicket, &app,
                   [=](const Ticket& ticket) {
                     player->addTicketToPlaylist(ticket);
                     model->addTicket(ticket);
                   });
  auto* processingTimer = new QTimer(&app);
  processingTimer->setInterval(5000);
  QObject::connect(processingTimer, &QTimer::timeout, &app, [=]() {
    if (player->state() == QMediaPlayer::State::StoppedState) {
      processor->sendGetTicketRequest();
    }
  });
  QObject::connect(&settings, &AppSettings::connectionSettingsChanged, &app,
                   [=]() {
                     processingTimer->stop();
                     processor->login();
                   });

  auto* loginTimer = new QTimer(&app);
  QObject::connect(processor, &TicketProcessor::loginSuccess, [=]() {
    loginTimer->stop();
    processingTimer->start();
  });
  QObject::connect(processor, SIGNAL(needLogin(int)), loginTimer,
                   SLOT(start(int)));
  QObject::connect(loginTimer, SIGNAL(timeout()), processor, SLOT(login()));
  processor->login();

  auto* cleanerTimer = new QTimer(&app);
  QObject::connect(cleanerTimer, SIGNAL(timeout()), model,
                   SLOT(deleteOldestTicket()));
  cleanerTimer->start(10'000);

  return app.exec();
}
