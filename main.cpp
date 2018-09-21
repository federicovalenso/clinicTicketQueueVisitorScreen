#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

#include "ticket.h"
#include "ticketmodel.h"
#include "ticketsplayer.h"
#include "ticketprocessor.h"

using namespace  std;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    app.setOrganizationName("MANO \'MDC\'");
    app.setApplicationName("Mano \'MDC\' Ticket\'s scoreboard");

    TicketModel* model = new TicketModel(&app);
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("ticketsModel", model);
    engine.load(QUrl(QStringLiteral("qrc:/views/ticketsView.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    TicketsPlayer* player = new TicketsPlayer(&app);
    TicketProcessor* processor = new TicketProcessor(&app);
    //TODO: получать талоны не по таймеру, а после окончания воспроизведения номера талона
    QObject::connect(processor,
            &TicketProcessor::receivedTicket,
            &app,
            [=](const Ticket& ticket)
            {
                player->addTicketToPlaylist(ticket);
                model->addTicket(ticket);
            });
    QTimer* ticketsTimer = new QTimer(&app);
    QObject::connect(ticketsTimer,
            &QTimer::timeout,
            &app,
            [=]() {processor->sendGetTicketRequest();});
    ticketsTimer->start(5000);

    return app.exec();
}
