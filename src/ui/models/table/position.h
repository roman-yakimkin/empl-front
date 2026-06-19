//
// Created by Roman Yakimkin on 17.06.2026.
//

#pragma once

#include <QAbstractTableModel>

#include "core/models/position.h"

namespace views::models::table {
    class Position : public QAbstractTableModel {
        Q_OBJECT

        QVector<model::Position> m_items;
    public:
        explicit Position(QObject* parent = nullptr);

        int rowCount(const QModelIndex &parent) const override;
        int columnCount(const QModelIndex &parent) const override;
        QVariant data(const QModelIndex &index, int role) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

        void setItems(const QVector<model::Position> &items);
        void addItem(const model::Position& item);
        void updateItem(const model::Position& item);
        void deleteItem(int id);

        [[nodiscard]] int getIdByRow(int row) const;
        [[nodiscard]] model::Position getByRow(int row) const;

    };
}