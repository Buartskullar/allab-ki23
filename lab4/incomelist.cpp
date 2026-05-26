#include "incomelist.h"

incomeList::incomeList(QObject *parent) : QAbstractListModel(parent) {

}

int incomeList::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return m_data.count();
}

QVariant incomeList::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_data.count())
        return QVariant();

    const income &obj = m_data.at(index.row());

    if (role == Qt::DisplayRole) {
        return obj.getName();
    }
    return QVariant();
}

void incomeList::addObject(const income &obj) {
    beginInsertRows(QModelIndex(), m_data.count(), m_data.count());
    m_data.append(obj);
    endInsertRows();
}

void incomeList::removeObject(const QModelIndex &index) {
    if (!index.isValid()) return;

    int row = index.row();
    if (row < 0 || row >= m_data.count()) return;

    beginRemoveRows(QModelIndex(), index.row(), index.row());
    m_data.removeAt(index.row());
    endRemoveRows();
}

income incomeList::getItem(const QModelIndex &index) const {
    if (index.isValid() && index.row() >= 0 && index.row() < m_data.count()) {
        return m_data.at(index.row());
    }
    return income();
}

bool incomeList::setData(const QModelIndex &index, const QVariant &value, int role){
    if (!index.isValid() || index.row() >= m_data.size())
        return false;

    income &item = m_data[index.row()];

    switch (role) {
    case NameRole:
    case Qt::EditRole:
        item.setName(value.toString());
        break;
    case DayRole:
        item.setDay(value.toString());
        break;
    case PlaceRole:
        item.setPlace(value.toString());
        break;
    case SumRole:
        item.setSum(value.toInt());
        break;
    default:
        return false;
    }

    emit dataChanged(index, index, {role});
    return true;
}

void incomeList::filedLoad(){
    if (filer.status){
        beginResetModel();
        m_data.clear();
        m_data = fileworks::loadFromFile();
    }
    endResetModel();
}

void incomeList::filedSave(){
    fileworks::saveToFile(m_data);
}

void incomeList::execCommands(){
    beginResetModel();
    int comAmount = fileworks::comCount();
    for (int i = 0; i < comAmount; i++){
        QString curCom = fileworks::getCommand(i);
        if (curCom.sliced(0,3) == "ADD"){
            comAdd(curCom.sliced(3));
        }
        else if (curCom.sliced(0,3) == "REM"){
            comRem(curCom.sliced(3));
        }
        else if (curCom.sliced(0,4) == "SAVE"){
            comSave(curCom.sliced(4));
        }
    }
    endResetModel();
}

void incomeList::comAdd(QString curCom){
    static QRegularExpression re("^(.*):(\\d{2}\\.\\d{2}\\.\\d{4})\\s+\"(.*)\"\\s+(-?\\d+)$");
    QRegularExpressionMatch match = re.match(curCom);

    if (match.hasMatch()) {
        income obj;
        obj.setName(match.captured(1).trimmed());
        obj.setDay(match.captured(2));
        obj.setPlace(match.captured(3));
        obj.setSum(match.captured(4).toInt());

        m_data.append(obj);
    }
}

void incomeList::comRem(QString curCom){
    QRegularExpression re("^(\\w+)\\s*([<>=!]+)\\s*(\\d{2}\\.\\d{2}\\.\\d{4}|\\d+)$");
    QString fieldName;
    QString op;
    QString value;
    QRegularExpressionMatch match = re.match(curCom.trimmed());

    if (match.hasMatch()) {
        fieldName = match.captured(1); // Первая группа: название
        op = match.captured(2);        // Вторая группа: знак
        value = match.captured(3); // Третья группа: число
    }

    for (int index = 0; index < m_data.size(); index++){
        if (fieldName == "Day"){
            dayComp(index, op, value);
        }
        else if (fieldName == "Sum"){
            sumComp(index, op, value);
        }
    }
}

void incomeList::comSave(QString curCom){
    fileworks::saveToFile(m_data, curCom);
}

void incomeList::dayComp(int index, QString op, QString value){
    QString d1 = m_data[index].getDay(); // "05.12.2024"
    QString d2 = value;         // "01.10.2024"
    // Чтобы сравнить строки как даты, нужно переставить части: YYYYMMDD
    auto toSortable = [](QString s) {
        QStringList p = s.split('.');
        if (p.size() != 3) return QString("");
        return p[2] + p[0] + p[1]; // Год + Месяц + День
    };

    QString s1 = toSortable(d1);
    QString s2 = toSortable(d2);

    if (s1.isEmpty() || s2.isEmpty()) return;

    if ((op == ">" && s1 > s2) ||
        ((op == "=" || op == "==") && s1 == s2) ||
        (op == "<" && s1 < s2))
    {
        removeObject(createIndex(index,0));
    }
}

void incomeList::sumComp(int index, QString op, QString value){
    int s1 = m_data[index].getSum();
    int s2 = value.toInt();
    if ((op == ">" && s1 > s2) ||
        ((op == "=" || op == "==") && s1 == s2) ||
        (op == "<" && s1 < s2))
    {
        removeObject(createIndex(index,0));
    }
}
