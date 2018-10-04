#ifndef TICKETNUMBERPARSER_H
#define TICKETNUMBERPARSER_H

#include <vector>
#include <QString>

class TicketNumberParser
{
public:
    TicketNumberParser() = default;
    static std::vector<QString> parse(const QString& ticket_number);
};

#endif // TICKETNUMBERPARSER_H
