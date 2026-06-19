//
// Created by Roman Yakimkin on 14.06.2026.
//

#pragma once

#include <qabstractitemmodel.h>

#include "core/models/department.h"

namespace views::models::table {
    class Department : public QAbstractTableModel {
        Q_OBJECT

        QVector<model::Department> m_items;
    public:
        explicit Department(QObject* parent = nullptr);

        int rowCount(const QModelIndex &parent) const override;
        int columnCount(const QModelIndex &parent) const override;
        QVariant data(const QModelIndex &index, int role) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

        void setItems(const QVector<model::Department> &items);
        void addItem(const model::Department& item);
        void updateItem(const model::Department& item);
        void deleteItem(int id);

        [[nodiscard]] int getIdByRow(int row) const;
        [[nodiscard]] model::Department getByRow(int row) const;
    };
} // views::models::table