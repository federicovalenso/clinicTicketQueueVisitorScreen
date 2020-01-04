#include "ticketsplayertest.h"
#include <QString>
#include "../ticketnumberparser.h"

TicketsPlayerTest::TicketsPlayerTest(QObject *parent) : QObject(parent) {}

void TicketsPlayerTest::testParseTicketNumber() {
  using namespace std;
  QFETCH(QString, input);
  QFETCH(vector<QString>, result);

  QCOMPARE(TicketNumberParser::parse(input), result);
}

void TicketsPlayerTest::testParseTicketNumber_data() {
  using namespace std;
  QTest::addColumn<QString>("input");
  QTest::addColumn<vector<QString>>("result");

  QTest::addRow("Б1") << QString("Б1") << vector<QString>({"б", "1"});
  QTest::addRow("Б7") << QString("Б7") << vector<QString>({"б", "7"});
  QTest::addRow("Б543") << QString("Б543")
                        << vector<QString>({"б", "500", "40", "3"});
  QTest::addRow("Б22") << QString("Б22") << vector<QString>({"б", "20", "2"});
  QTest::addRow("Э122") << QString("Э122")
                        << vector<QString>({"э", "100", "20", "2"});
  QTest::addRow("Б10") << QString("Б10") << vector<QString>({"б", "10"});
  QTest::addRow("В15") << QString("В15") << vector<QString>({"в", "15"});
  QTest::addRow("В100") << QString("В100") << vector<QString>({"в", "100"});
  QTest::addRow("В30") << QString("В30") << vector<QString>({"в", "30"});
  QTest::addRow("А257") << QString("А257")
                        << vector<QString>({"а", "200", "50", "7"});
  QTest::addRow("А200") << QString("А200") << vector<QString>({"а", "200"});
}

QTEST_MAIN(TicketsPlayerTest)
