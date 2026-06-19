//
// Created by Roman Yakimkin on 18.06.2026.
//

#include "employee.h"

namespace views::models::table {
    Employee::Employee(QObject *parent) : QAbstractTableModel(parent) {}

    int Employee::rowCount(const QModelIndex &parent) const {
        return m_items.size();
    }

    int Employee::columnCount(const QModelIndex &parent) const {
        return 6;
    }

    QVariant Employee::data(const QModelIndex &index, int role) const {
        if (!index.isValid() || index.row() >= m_items.size()) return {};

        const auto& item = m_items[index.row()];

        if (role == Qt::DisplayRole) {
            switch (index.column()) {
                case 0: return QString::number(item.id);
                case 1: return item.name;
                case 2: return item.department.name;
                case 3: return item.position.name;
                case 4: return item.created_at.toString("dd.MM.yyyy HH:mm:ss");
                case 5: return item.updated_at.toString("dd.MM.yyyy HH:mm:ss");
                default: return {};
            }
        }

        if (role == Qt::UserRole)  return item.id;

        return {};
    }

    QVariant Employee::headerData(int section, Qt::Orientation orientation, int role) const {
        if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
            switch (section) {
                case 0: return "ID";
                case 1: return "ФИО";
                case 2: return "Отдел";
                case 3: return "Должность";
                case 4: return "Дата\nсоздания";
                case 5: return "Дата последнего\nизменения";
                default: return {};
            }
        }

        return QAbstractTableModel::headerData(section, orientation, role);
    }

    void Employee::setItems(const QVector<model::Employee> &items) {
        beginResetModel();
        m_items = items;
        endResetModel();
    }

    void Employee::addItem(const model::Employee &item) {
        const auto mi = QModelIndex();
        beginInsertRows(mi, rowCount(mi), rowCount(mi));
        m_items.push_back(item);
        endInsertRows();
    }

    void Employee::updateItem(const model::Employee &item) {
        for (int i = 0; i < m_items.size(); i++) {
            if (m_items[i].id == item.id) {
                m_items[i] = item;
                emit dataChanged(index(i, 0), index(i, columnCount(QModelIndex()) - 1));

                return;
            }
        }
    }

    void Employee::deleteItem(int id) {
        for (int i = 0; i < m_items.size(); i++) {
            if (m_items[i].id == id) {
                beginRemoveRows(QModelIndex(), i, i);
                m_items.erase(m_items.begin() + i);
                endRemoveRows();

                return;
            }
        }
    }

    int Employee::getIdByRow(int row) const {
        if (row >= 0 && row < m_items.size()) return m_items[row].id;

        return -1;
    }

    model::Employee Employee::getByRow(int row) const {
        if (row >= 0 && row < m_items.size()) return m_items[row];

        return {};
    }

}
