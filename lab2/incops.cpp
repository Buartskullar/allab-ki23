#include "incops.h"

incops::incops() {}

QString incops::getName()   {
    return income_obj.name;
}

QString incops::getPlace()   {
    return income_obj.source_place;
}

QString incops::getDay()   {
    return income_obj.source_day;
}

int incops::getSum(){
    return income_obj.sum;
}

QList<QString> splitting(QString input){
    // 1. Сначала отделяем часть до первого двоеточия
    int firstColon = input.indexOf(':');
    if (firstColon == -1) return {}; // Или обработка ошибки

    QString firstPart = input.left(firstColon);
    QString rest = input.mid(firstColon + 1);

    // 2. Разбиваем оставшуюся часть по пробелам
    // Qt::SkipEmptyParts уберет лишние пробелы (аналог вашего if (end-start > 0))
    QList<QString> data = rest.split(' ', Qt::SkipEmptyParts);

    // 3. Добавляем первую часть в начало списка
    data.prepend(firstPart);

    return data;
}

incops::income incops::assign(const QList<QString> &input) {
    incops::income out;
    bool f_quo = false;

    for (const QString &item : input) {
        // 1. Если мы внутри кавычек — собираем source_place
        if (f_quo) {
            out.source_place += " " + item;
            // Если нашли закрывающую кавычку (и это не та же самая, что открывающая)
            if (item.contains('"')) {
                f_quo = false;
            }
            continue; // Переходим к следующему слову
        }

        // 2. Проверка на дату (наличие точки)
        if (item.contains('.')) {
            out.source_day = item;
        }
        // 3. Проверка на начало кавычек
        else if (item.contains('"')) {
            out.source_place = item;
            // Проверяем, есть ли вторая кавычка в этом же слове
            // count() помогает понять, одиночная кавычка или пара
            if (item.count('"') == 1) {
                f_quo = true;
            }
        }
        // 4. Логика для чисел и имен
        else {
            bool ok;
            //toInt(&ok) сам проверяет, является ли строка числом
            int val = item.toInt(&ok);

            if (ok) {
                out.sum = val;
            } else {
                out.name = item;
            }
        }
    }
    return out;
}
