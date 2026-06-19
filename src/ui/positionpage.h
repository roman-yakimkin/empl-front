//
// Created by Roman Yakimkin on 17.06.2026.
//

#pragma once

#include <QWidget>

#include <qabstractitemmodel.h>

#include "common/basepage.h"
#include "models/table/position.h"
#include "services/position.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class PositionPage;
}
QT_END_NAMESPACE

namespace views {
    class PositionPage : public common::BasePage {
        Q_OBJECT

    public:
        explicit PositionPage(core::IPositionSvc* svc, QWidget *parent = nullptr);

        ~PositionPage() override;

        void reloadData() override;
    private slots:
        void onBtnAddClicked();
        void onBtnDelClicked();
        void onBtnSaveClicked();
        void onBtnDelDoClicked();
        void onBtnCancelClicked();
        void onTableViewDblClicked(const QModelIndex& index);
        void onCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous);

        void onPositionsLoaded(const QVector<model::Position>& items);
        void onServiceError(const QString& errorCode, const QString& errorMessage);

        void positionCreated(const model::Position& item);
        void positionUpdated(const model::Position& item);
        void positionDeleted(int id);
    private:
        Ui::PositionPage *ui;

        core::IPositionSvc* m_svc;
        views::models::table::Position * m_model = nullptr;

        model::Position m_current = {};

        int m_widget_right_width_min = 0;
        int m_widget_right_width_max = 0;

        void setUpComponents();
        void setUpModel();
        void loadData();
        void setCurrentWidget(QWidget* current);
        void updateCurrent(const QModelIndex& index);

        bool editMode() const;
    };
}
