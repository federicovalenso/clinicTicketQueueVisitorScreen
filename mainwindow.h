#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <map>
#include <QMainWindow>
#include "ticket.h"
#include "ticketmodel.h"
#include "ticketsplayer.h"
#include "ticketprocessor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui;
    TicketsPlayer* player;
    TicketProcessor* processor;
    QTimer* ticketsTimer;
    TicketModel* model;

    void updateTicketsTableSizes();
};

#endif // MAINWINDOW_H
