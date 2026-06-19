//
// Created by Roman Yakimkin on 14.06.2026.
//

#include "department.h"

namespace views::models::table {
    Department::Department(QObject *parent) : QAbstractTableModel(parent) {}

    int Department::rowCount(const QModelIndex &parent) const {
        return m_items.size();
    }

    int Department::columnCount(const QModelIndex &parent) const {
        return 2;
    }

    QVariant Department::data(const QModelIndex &index, int role) const {
        if (!index.isValid() || index.row() >= m_items.size()) return {};

        const auto& item = m_items[index.row()];

        if (role == Qt::DisplayRole) return index.column() == 0 ? QString::number(item.id) : item.name;

        if (role == Qt::UserRole)   return item.id;

        return {};
    }

    QVariant Department::headerData(int section, Qt::Orientation orientation, int role) const {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            return section == 0 ? "ID" : "Название";
        }

        return QAbstractTableModel::headerData(section, orientation, role);
    }

    void Department::setItems(const QVector<model::Department> &items) {
        beginResetModel();
        m_items = items;
        endResetModel();
    }

    void Department::addItem(const model::Department &item) {
        const auto mi = QModelIndex();
        beginInsertRows(mi, rowCount(mi), rowCount(mi));
        m_items.push_back(item);
        endInsertRows();
    }

    void Department::updateItem(const model::Department &item) {
        for (int i = 0; i < m_items.size(); i++) {
            if (m_items[i].id == item.id) {
                m_items[i] = item;
                emit dataChanged(index(i, 0), index(i, columnCount(QModelIndex()) - 1));

                return;
            }
        }
    }

    void Department::deleteItem(int id) {
        for (int i = 0; i < m_items.size(); i++) {
            if (m_items[i].id == id) {
                beginRemoveRows(QModelIndex(), i, i);
                m_items.erase(m_items.begin() + i);
                endRemoveRows();

                return;
            }
        }
    }

    int Department::getIdByRow(int row) const {
        if (row >= 0 && row < m_items.size()) return m_items[row].id;

        return -1;
    }

    model::Department Department::getByRow(int row) const {
        if (row >= 0 && row < m_items.size()) return m_items[row];

        return {};
    }
} // ui::models::table