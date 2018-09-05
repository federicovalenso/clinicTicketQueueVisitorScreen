#ifndef TICKETMODEL_H
#define TICKETMODEL_H

#include <QAbstractTableModel>
#include <vector>
#include <QString>
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
    void addTicket(const Ticket &ticket);

private:
    std::vector<Ticket> data_;
    void addRow(const Ticket& ticket);
};

#endif // TICKETMODEL_H
