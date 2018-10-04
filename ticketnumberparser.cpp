#include <QRegExp>
#include "ticketnumberparser.h"

std::vector<QString> TicketNumberParser::parse(const QString &ticket_number)
{
    using namespace std;
    auto digitPos = ticket_number.indexOf(QRegExp("[1-9]"));
    vector<QString> result;
    QString letter = ticket_number.left(digitPos).toLower();
    if (letter.isEmpty() == false) {
        result.push_back(std::move(letter));
        int number = ticket_number.mid(digitPos).toInt();
        int hundreds = number / 100;
        if (hundreds > 0) {
            result.push_back(QString::number(hundreds * 100));
        }
        int minusHundreds = number - hundreds * 100;
        if (minusHundreds > 10 && minusHundreds < 20) {
            result.push_back(QString::number(minusHundreds));
        } else {
            int tens = minusHundreds / 10;
            int ones = number % 10;
            if (tens > 0) {
                result.push_back(QString::number(tens * 10));
            }
            if (ones > 0) {
                result.push_back(QString::number(ones));
            }
        }
    }
    return result;
}
