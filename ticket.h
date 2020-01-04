#ifndef TICKET_H
#define TICKET_H

#include <QString>
#include <QTime>
#include <chrono>

struct Ticket {
  int id = -1;
  int window = -1;
  QString ticket_number;
  std::chrono::steady_clock::time_point created_at =
      std::chrono::steady_clock::now();
};

inline bool operator==(const Ticket& lhs, const Ticket& rhs) {
  return lhs.ticket_number == rhs.ticket_number && lhs.window == rhs.window;
}

inline bool operator!=(const Ticket& lhs, const Ticket& rhs) {
  return !(lhs == rhs);
}

#endif  // TICKET_H
