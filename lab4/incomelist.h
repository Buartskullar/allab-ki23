#ifndef INCOMELIST_H
#define INCOMELIST_H

//#include "income.h"
#include "fileworks.h"
#include <QAbstractListModel>
#include <QList>
#include <QFile>
#include <QRegularExpression>
#include <QDate>

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

    void filedLoad();
    void filedSave();

    void execCommands();
private:
    QList<income> m_data;
    fileworks filer;
    void comAdd(QString curCom);
    void comRem(QString curCom);
    void comSave(QString curCom);
    void dayComp(int index, QString op, QString value);
    void sumComp(int index, QString op, QString value);
};

#endif // INCOMELIST_H
