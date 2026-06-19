//
// Created by Roman Yakimkin on 18.06.2026.
//

#pragma once
#include <QAbstractTableModel>

#include "core/models/employee.h"

namespace views::models::table {
    class Employee : public QAbstractTableModel {
        Q_OBJECT

        QVector<model::Employee> m_items;
    public:
        explicit Employee(QObject* parent = nullptr);

        int rowCount(const QModelIndex &parent) const override;
        int columnCount(const QModelIndex &parent) const override;
        QVariant data(const QModelIndex &index, int role) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

        void setItems(const QVector<model::Employee> &items);
        void addItem(const model::Employee& item);
        void updateItem(const model::Employee& item);
        void deleteItem(int id);

        [[nodiscard]] int getIdByRow(int row) const;
        [[nodiscard]] model::Employee getByRow(int row) const;
    };
}
