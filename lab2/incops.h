#ifndef INCOPS_H
#define INCOPS_H
#include <QApplication>

class incops
{
public:
    incops();

    QString getName();
    QString getDay();
    QString getPlace();
    int getSum();

private:
    struct income
    {
        QString name;
        QString source_day;
        QString source_place;
        int sum;
    };

    income income_obj;
    QList<QString> splitting(QString input);
    income assign(const QList<QString> &input);
};

#endif // INCOPS_H
