#include "ticketmodel.h"
#include <QColor>
#include <QMutexLocker>
#include <algorithm>
#include <utility>

#include <QDebug>

TicketModel::TicketModel(QObject *parent, int max_tickets)
    : QAbstractTableModel(parent), max_tickets_(max_tickets) {}

int TicketModel::rowCount(const QModelIndex &) const { return data_.size(); }

int TicketModel::columnCount(const QModelIndex &) const { return 2; }

QVariant TicketModel::data(const QModelIndex &index, int role) const {
  QVariant result;
  if (role == Roles::TICKET_NUMBER) {
    result = data_.at(index.row()).ticket_number;
  } else if (role == Roles::WINDOW) {
    result = data_.at(index.row()).window;
  }
  return result;
}

QVariant TicketModel::headerData(int section, Qt::Orientation orientation,
                                 int role) const {
  if (role == Qt::DisplayRole) {
    if (orientation == Qt::Horizontal) {
      if (section == 0) {
        return QString(tr("Талон"));
      } else if (section == 1) {
        return QString(tr("Окно"));
      }
    }
  }
  return QVariant();
}

bool TicketModel::setData(const QModelIndex &index, const QVariant &value,
                          int role) {
  if (index.isValid() == true && role == Qt::EditRole) {
    if (index.column() == 0) {
      data_[index.row()].ticket_number = value.toString();
    } else if (index.column() == 1) {
      data_[index.row()].window = value.toInt();
    }
    emit dataChanged(index, index);
    return true;
  }
  return false;
}

Qt::ItemFlags TicketModel::flags(const QModelIndex &index) const {
  return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

bool TicketModel::removeRows(int row, int count, const QModelIndex &parent) {
  if (data_.size() > 0) {
    auto last = row + count;
    beginRemoveRows(parent, row, last - 1);
    if (count == 1) {
      data_.erase(data_.begin() + row);
    } else if (count > 1) {
      data_.erase(data_.begin() + row, data_.begin() + last);
    }
    endRemoveRows();
    return true;
  }
  return false;
}

QHash<int, QByteArray> TicketModel::roleNames() const {
  return {{static_cast<int>(Roles::TICKET_NUMBER), "number"},
          {static_cast<int>(Roles::WINDOW), "window"}};
}

void TicketModel::addTicket(const Ticket &ticket) {
  QMutexLocker locker(&mutex_);
  if (data_.size() > 0) {
    if (std::find(data_.begin(), data_.end(), ticket) == data_.end()) {
      removeRepetitiveTickets(ticket);
      if (data_.size() == max_tickets_) {
        removeRows();
      }
      addRow(ticket);
    }
  } else {
    addRow(ticket);
  }
}

int TicketModel::getMaxTicketsCount() const { return max_tickets_; }

void TicketModel::changeMaxTicket(int max_tickets) {
  max_tickets_ = max_tickets;
  QMutexLocker locker(&mutex_);
  int diff = data_.size() - max_tickets;
  if (diff > 0) {
    removeRows(0, diff);
  }
  emit maxTicketChanged(max_tickets);
}

void TicketModel::addRow(const Ticket &ticket) {
  beginInsertRows(QModelIndex(), data_.size(), data_.size());
  data_.push_back(ticket);
  endInsertRows();
}

QVector<Ticket>::Iterator TicketModel::findRepetitiveTicket(
    const Ticket &ticket) {
  return std::find_if(
      data_.begin(), data_.end(), [=](const Ticket &cur_ticket) {
        return cur_ticket.ticket_number == ticket.ticket_number ||
               cur_ticket.window == ticket.window;
      });
}

void TicketModel::removeRepetitiveTickets(const Ticket &ticket) {
  auto find_ticket_it = findRepetitiveTicket(ticket);
  while (find_ticket_it != data_.end()) {
    removeRows(find_ticket_it - data_.begin());
    find_ticket_it = findRepetitiveTicket(ticket);
  }
}
