#include <QApplication>

#include "mainwindow.h"

using namespace  std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle("Fusion");
    app.setOrganizationName("MANO \'MDC\'");
    app.setApplicationName("Mano \'MDC\' Server App");
    MainWindow w;
    w.show();
    return app.exec();
}
