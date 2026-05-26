#ifndef INCOMELIST_H
#define INCOMELIST_H

#include "income.h"
#include <QAbstractListModel>
#include <QList>
#include <QFile>
#include <QRegularExpression>

class incomeList : public QAbstractListModel
{
    Q_OBJECT

public:
    enum IncomeRoles {
        NameRole = Qt::UserRole + 1,
        DayRole,
        PlaceRole,
        SumRole
    };

    explicit incomeList(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const ;

    void addObject(const income &obj = income());
    void removeObject(const QModelIndex &index);
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    income getItem(const QModelIndex &index) const;

    void loadFromFile();
    void saveToFile();

private:
    QList<income> m_data;
};

#endif // INCOMELIST_H
