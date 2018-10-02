#ifndef TICKETMODEL_H
#define TICKETMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QString>
#include <QMutex>
#include "ticket.h"

class TicketModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    TicketModel(QObject* parent = nullptr);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex&) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual bool removeRows(int row = 0, int count = 1, const QModelIndex &parent = QModelIndex()) override;
    virtual QHash<int, QByteArray> roleNames() const override;
    void addTicket(const Ticket &ticket);

    enum Roles
    {
        TICKET_NUMBER = Qt::ItemDataRole::UserRole + 1,
        WINDOW
    };

private:
    const QVector<Ticket>::size_type MAX_TICKETS = 4;
    QVector<Ticket> data_;
    QMutex mutex_;
    void addRow(const Ticket& ticket);
    QVector<Ticket>::Iterator findRepetitiveTicket(const Ticket& ticket);
    void removeRepetitiveTickets(const Ticket& ticket);
};

#endif // TICKETMODEL_H
