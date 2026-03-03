#ifndef INCOMELIST_H
#define INCOMELIST_H

#include "incops.h"
#include <QAbstractListModel>
#include <QList>

class incomeList : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit incomeList(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const ;

    void addObject(const incops::income &obj);
    void removeObject(int row);

private:
    QList<incops::income> m_data;
};

#endif // INCOMELIST_H
