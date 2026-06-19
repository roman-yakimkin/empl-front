//
// Created by Roman Yakimkin on 14.06.2026.
//

#pragma once

#include <QWidget>

#include <qabstractitemmodel.h>

#include "common/basepage.h"
#include "core/contracts/i_department_svc.h"
#include "core/contracts/i_position_svc.h"
#include "models/table/employee.h"
#include "services/employee.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class EmployeePage;
}
QT_END_NAMESPACE

namespace views {
    class EmployeePage : public common::BasePage {
        Q_OBJECT

    public:
        explicit EmployeePage(
            core::IEmployeeSvc* svc,
            core::IDepartmentSvc* dev_svc,
            core::IPositionSvc* pos_svc,
            QWidget *parent = nullptr
        );

        ~EmployeePage() override;

        void reloadData() override;
    private slots:
        void onBtnAddClicked();
        void onBtnDelClicked();
        void onBtnSaveClicked();
        void onBtnDelDoClicked();
        void onBtnCancelClicked();
        void onTableViewDblClicked(const QModelIndex& index);
        void onCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous);

        void onEmployeesLoaded(const QVector<model::Employee>& items);
        void onEmployeeLoaded(const model::Employee& item);
        void onServiceError(const QString& errorCode, const QString& errorMessage);

        void employeeCreated(const model::Employee& item);
        void employeeUpdated(const model::Employee& item);
        void employeeDeleted(int id);

        void onDepartmentsLoaded(const QVector<model::Department> &items);
        void onDepartmentCreated(const model::Department &item);
        void onDepartmentDeleted(int id);
        void onBtnAddDepartmentClicked();

        void onPositionsLoaded(const QVector<model::Position> &items);
        void onPositionCreated(const model::Position &item);
        void onPositionDeleted(int id);
        void onBtnAddPositionClicked();
    private:
        Ui::EmployeePage *ui;

        core::IEmployeeSvc* m_svc;
        core::IDepartmentSvc *m_dep_svc;
        core::IPositionSvc *m_pos_svc;

        views::models::table::Employee * m_model = nullptr;

        model::Employee m_current = {};
        QVector<model::Department> m_departments;
        QVector<model::Position> m_positions;

        int m_widget_right_width_min = 0;
        int m_widget_right_width_max = 0;

        void setupComponents();
        void setupModel();
        void loadData();
        void setCurrentWidget(QWidget* current);
        void updateCurrent(const QModelIndex& index);
        bool editMode() const;

        void loadDepartments();
        void fillDepartmentsComboBox();
        int getSelectedDepartmentId() const;
        void selectDepartmentInComboBox(int dept_id);

        void loadPositions();
        void fillPositionsComboBox();
        int getSelectedPositionId() const;
        void selectPositionInComboBox(int dept_id);
    };
}
