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

    income transformToIncome(QString input);


private:
    income income_obj;
    QList<QString> splitting(QString input);
    income assign(QList<QString> input);
};

#endif // INCOPS_H
