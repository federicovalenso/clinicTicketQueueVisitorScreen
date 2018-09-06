#include <utility>
#include <QColor>
#include "ticketmodel.h"

TicketModel::TicketModel(QObject *parent)
    : QAbstractTableModel (parent)
{}

int TicketModel::rowCount(const QModelIndex&) const
{
    return data_.size();
}

int TicketModel::columnCount(const QModelIndex&) const
{
    return 2;
}

QVariant TicketModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        if (index.column() == 0) {
            return data_.at(index.row()).ticket_number;
        } else if (index.column() == 1) {
            return data_.at(index.row()).window;
        }
    } else if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    } else if (role == Qt::TextColorRole) {
        return QColor(52, 31, 151);
    } else if (role == Qt::BackgroundRole) {
        QColor color;
        if (index.row() % 2 == 0) {
            color.setRgb(72, 219, 251);
        } else {
            color.setRgb(0, 210, 211);
        }
        color.setAlpha(70);
        return color;
    }
    return QVariant();
}

QVariant TicketModel::headerData(int section, Qt::Orientation orientation, int role) const
{
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

bool TicketModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() == true && role == Qt::EditRole) {
        if (index.column() == 0) {
            data_[index.row()].ticket_number = value.toString();
        } else if (index.column() == 1){
            data_[index.row()].window = value.toInt();
        }
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags TicketModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

bool TicketModel::removeRows(int, int, const QModelIndex &parent)
{
    if (data_.size() > 0) {
        beginRemoveRows(parent, 0, 0);
        data_.erase(data_.begin());
        endRemoveRows();
        return true;
    }
    return false;
}

void TicketModel::addTicket(const Ticket& ticket)
{
    if (data_.size() > 0) {
        if (data_.back() != ticket) {
            addRow(ticket);
            if (data_.size() > MAX_TICKETS) {
                removeRows();
            }
        }
    } else {
        addRow(ticket);
    }
}

void TicketModel::addRow(const Ticket &ticket)
{
    beginInsertRows(QModelIndex(), data_.size(), data_.size());
    data_.push_back(ticket);
    endInsertRows();
}
