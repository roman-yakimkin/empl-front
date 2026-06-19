//
// Created by Roman Yakimkin on 14.06.2026.
//

#pragma once

#include <QWidget>

#include <qabstractitemmodel.h>

#include "common/basepage.h"
#include "models/table/department.h"
#include "services/department.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class DepartmentPage;
}
QT_END_NAMESPACE

namespace views {
    class DepartmentPage : public common::BasePage {
        Q_OBJECT

    public:
        explicit DepartmentPage(core::IDepartmentSvc* svc, QWidget *parent = nullptr);

        ~DepartmentPage() override;

        void reloadData() override;
    private slots:
        void onBtnAddClicked();
        void onBtnDelClicked();
        void onBtnSaveClicked();
        void onBtnDelDoClicked();
        void onBtnCancelClicked();
        void onTableViewDblClicked(const QModelIndex& index);
        void onCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous);

        void onDepartmentsLoaded(const QVector<model::Department>& items);
        void onServiceError(const QString& errorCode, const QString& errorMessage);

        void departmentCreated(const model::Department& item);
        void departmentUpdated(const model::Department& item);
        void departmentDeleted(int id);
    private:
        Ui::DepartmentPage *ui;

        core::IDepartmentSvc* m_svc;
        views::models::table::Department * m_model = nullptr;

        model::Department m_current = {};

        int m_widget_right_width_min = 0;
        int m_widget_right_width_max = 0;

        void setupComponents();
        void setupModel();
        void loadData();
        void setCurrentWidget(QWidget* current);
        void updateCurrent(const QModelIndex& index);

        bool editMode() const;
    };
}
