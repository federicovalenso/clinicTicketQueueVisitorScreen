#ifndef TICKET_H
#define TICKET_H

#include <QString>

struct Ticket
{
    int id = -1;
    QString ticket_number;
    int window = -1;

    inline bool isValid()
    {
        return id != -1 ? true : false;
    }
};

#endif // TICKET_H
