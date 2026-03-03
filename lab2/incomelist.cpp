#include "incomelist.h"

incomeList::incomeList(QObject *parent) : QAbstractListModel(parent) {}

int incomeList::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return m_data.count();
}

QVariant incomeList::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_data.count())
        return QVariant();

    const incops::income &obj = m_data.at(index.row());

    if (role == Qt::DisplayRole) {
        return obj.name; // Что именно будет текстом в списке
    }
    // Можно добавить другие роли, например ToolTipRole
    return QVariant();
}

void incomeList::addObject(const incops::income &obj) {
    // Сообщаем модели, что сейчас добавится новая строка
    beginInsertRows(QModelIndex(), m_data.count(), m_data.count());
    m_data.append(obj);
    endInsertRows();
}

void incomeList::removeObject(int row) {
    if (row < 0 || row >= m_data.count()) return;

    beginRemoveRows(QModelIndex(), row, row);
    m_data.removeAt(row);
    endRemoveRows();
}
