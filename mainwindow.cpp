#include <vector>
#include <QTimer>
#include <QJsonObject>
#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new TicketModel(this);
    ui->tvTickets->setModel(model);
    updateTicketsTableSizes();
    QTimer::singleShot(1000, this, SLOT(showFullScreen()));
    player = new TicketsPlayer(this);
    processor = new TicketProcessor(this);
    connect(processor,
            &TicketProcessor::receivedTicket,
            this,
            [=](const Ticket& ticket)
            {
                player->addTicketToPlaylist(ticket);
                model->addTicket(ticket);
                this->updateTicketsTableSizes();
            });
    ticketsTimer = new QTimer(this);
    connect(ticketsTimer,
            &QTimer::timeout,
            this,
            [=]() {processor->sendGetTicketRequest();});
    ticketsTimer->start(5000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    updateTicketsTableSizes();
    QMainWindow::resizeEvent(event);
}

void MainWindow::updateTicketsTableSizes()
{
    int new_width = ui->tvTickets->width() / 2 - 10;
    ui->tvTickets->setColumnWidth(0, new_width);
    ui->tvTickets->setColumnWidth(1, new_width);
    ui->tvTickets->resizeRowsToContents();
}
