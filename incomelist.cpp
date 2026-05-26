#include "incomelist.h"

incomeList::incomeList(QObject *parent) : QAbstractListModel(parent) {}

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

void incomeList::loadFromFile(){
    QFile file("C:/CODE/OSNOV/lab2/data.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    beginResetModel();
    m_data.clear();

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    //выделить в отдельную функцию для написания модульных тестов
    static QRegularExpression re("^(.*):(\\d{2}\\.\\d{2}\\.\\d{4})\\s+\"(.*)\"\\s+(-?\\d+)$");

    while (!in.atEnd()) {
        QString line = in.readLine();
        QRegularExpressionMatch match = re.match(line);

        if (match.hasMatch()) {
            income obj;
            obj.setName(match.captured(1));
            obj.setDay(match.captured(2));
            obj.setPlace(match.captured(3));
            obj.setSum(match.captured(4).toInt());

            m_data.append(obj);
        }
    }

    file.close();
    endResetModel();
}

void incomeList::saveToFile() {
    QFile file("C:/CODE/OSNOV/lab2/data.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    for (const income &item : m_data) {
        out << item.getName() << ":"
            << item.getDay() << " "
            <<"\"" << item.getPlace() << "\" "
            << item.getSum() << "\n";
    }
    file.close();
}
