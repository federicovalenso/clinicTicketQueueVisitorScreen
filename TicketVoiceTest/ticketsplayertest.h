#ifndef TICKETSPLAYERTEST_H
#define TICKETSPLAYERTEST_H

#include <QObject>
#include <QtTest/QtTest>

class TicketsPlayerTest : public QObject
{
    Q_OBJECT
public:
    explicit TicketsPlayerTest(QObject *parent = nullptr);

private slots:
    void testParseTicketNumber();
    void testParseTicketNumber_data();
};

#endif // TICKETSPLAYERTEST_H
