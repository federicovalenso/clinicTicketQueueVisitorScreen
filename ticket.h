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

inline bool operator == (const Ticket& lhs, const Ticket& rhs)
{
    return lhs.ticket_number == rhs.ticket_number && lhs.window == rhs.window;
}

inline bool operator != (const Ticket& lhs, const Ticket& rhs)
{
    return !(lhs == rhs);
}

#endif // TICKET_H
