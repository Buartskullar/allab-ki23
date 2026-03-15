#include "income.h"

income::income() {
    source_day = "not_def";
    source_place = "not_def";
    source_sum = 0;
    source_name = "empty";
}

QString income::getName() const   {
    return source_name;
}

QString income::getPlace() const   {
    return source_place;
}

QString income::getDay() const   {
    return source_day;
}

int income::getSum() const{
    return source_sum;
}

void income::setDay(const QString day){
    source_day = day;
}
void income::setName(const QString name){
    source_name = name;
}
void income::setPlace(const QString place){
    source_place = place;
}
void income::setSum(const int sum){
    source_sum = sum;
}

income income::transformToIncome(const QString &input){
    return assign(splitting(input));
}


QList<QString> income::splitting(QString input){
    int firstColon = input.indexOf(':');
    if (firstColon == -1) return {};

    QString firstPart = input.left(firstColon);
    QString rest = input.mid(firstColon + 1);

    QList<QString> data = rest.split(' ', Qt::SkipEmptyParts);

    data.prepend(firstPart);

    return data;
}

income income::assign(QList<QString> input) {
    income out;
    bool f_quo = false;

    for (const QString &item : input) {
        if (f_quo) {
            out.source_place += " " + item;
            if (item.contains('"')) {
                f_quo = false;
            }
            continue;
        }

        if (item.contains('.')) {
            out.source_day = item;
        }

        else if (item.contains('"')) {
            out.source_place = item;
            if (item.count('"') == 1) {
                f_quo = true;
            }
        }
        else {
            bool ok;
            int val = item.toInt(&ok);

            if (ok) {
                out.source_sum = val;
            } else {
                out.source_name = item;
            }
        }
    }
    return out;
}
