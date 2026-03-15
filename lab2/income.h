#ifndef INCOME_H
#define INCOME_H
#include <QString>
#include <QList>

class income
{
public:
    income();
    income(QString name, QString source_day, QString source_place, int sum);

    static income transformToIncome(const QString &input);

    QString getName() const;
    QString getDay() const;
    QString getPlace() const;
    int getSum() const;

    void setName(const QString name);
    void setDay(const QString day);
    void setPlace(const QString place);
    void setSum(const int sum);


private:
    QString source_name;
    QString source_day;
    QString source_place;
    int source_sum;

    static QList<QString> splitting(QString input);
    static income assign(QList<QString> input);
};

#endif // INCOME_H
