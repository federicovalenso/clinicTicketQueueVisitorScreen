#ifndef TICKETMODEL_H
#define TICKETMODEL_H

#include <QAbstractTableModel>
#include <QMutex>
#include <QString>
#include <QVector>
#include <atomic>
#include "ticket.h"

class TicketModel : public QAbstractTableModel {
  Q_OBJECT

 public:
  TicketModel(QObject *parent = nullptr, int max_tickets = 4);
  virtual int rowCount(
      const QModelIndex &parent = QModelIndex()) const override;
  virtual int columnCount(const QModelIndex &) const override;
  virtual QVariant data(const QModelIndex &index, int role) const override;
  virtual QVariant headerData(int section, Qt::Orientation orientation,
                              int role) const override;
  virtual bool setData(const QModelIndex &index, const QVariant &value,
                       int role) override;
  virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
  virtual bool removeRows(int row = 0, int count = 1,
                          const QModelIndex &parent = QModelIndex()) override;
  virtual QHash<int, QByteArray> roleNames() const override;

  void addTicket(const Ticket &ticket);
  Q_INVOKABLE int getMaxTicketsCount() const;

  enum Roles { TICKET_NUMBER = Qt::ItemDataRole::UserRole + 1, WINDOW };

 signals:
  void maxTicketChanged(int number);

 public slots:
  void changeMaxTicket(int max_tickets);

 private:
  std::atomic<int> max_tickets_;
  QVector<Ticket> data_;
  QMutex mutex_;

  void addRow(const Ticket &ticket);
  QVector<Ticket>::Iterator findRepetitiveTicket(const Ticket &ticket);
  void removeRepetitiveTickets(const Ticket &ticket);
};

#endif  // TICKETMODEL_H
