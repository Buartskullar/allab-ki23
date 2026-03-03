#ifndef INCOPS_H
#define INCOPS_H
#include <QApplication>

class incops
{
public:
    incops();

    struct income
    {
        QString name;
        QString source_day;
        QString source_place;
        int sum;
    };

    QString getName();
    QString getDay();
    QString getPlace();
    int getSum();

    static income transformToIncome(const QString input);


private:
    income income_obj;
    static QList<QString> splitting(QString input);
    static income assign(QList<QString> input);
};

#endif // INCOPS_H
