#ifndef TICKETNUMBERPARSER_H
#define TICKETNUMBERPARSER_H

#include <QString>
#include <vector>

namespace TicketNumberParsing {
std::vector<QString> parse(const QString& ticket_number);
};

#endif  // TICKETNUMBERPARSER_H
