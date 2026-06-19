//
// Created by Roman Yakimkin on 11.06.2026.
//
#pragma once

#include <QMainWindow>
#include <QStackedWidget>

#include "departmentpage.h"
#include "employeepage.h"
#include "positionpage.h"
#include "core/contracts/i_department_svc.h"
#include "core/contracts/i_employee_svc.h"
#include "core/contracts/i_position_svc.h"
#include "infrastructure/appconfig.h"
#include "infrastructure/httpclient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

namespace views {
    class MainWindow : public QMainWindow {
        Q_OBJECT

        core::IDepartmentSvc* m_dep_svc = nullptr;
        core::IPositionSvc* m_pos_svc = nullptr;
        core::IEmployeeSvc* m_empl_svc = nullptr;
        infra::AppConfig* m_cfg = nullptr;
        core::IConfigurableClient* m_client = nullptr;

    protected:
        void closeEvent(QCloseEvent *event) override;

    public:
        explicit MainWindow(
            core::IDepartmentSvc* dep_svc,
            core::IPositionSvc* pos_svc,
            core::IEmployeeSvc* empl_svc,
            infra::AppConfig* cfg,
            core::IConfigurableClient* client,
            QWidget *parent = nullptr);

        ~MainWindow() override;

    private slots:
        void onStackWidgetChanged(int index);

        void onActionDepartmentsTriggered();
        void onActionPositionsTriggered();
        void onActionEmployeesTriggered();
        void onActionSettingsTriggered();

    private:
        Ui::MainWindow *ui;
        QStackedWidget *m_stack_widget;
        DepartmentPage *m_department_page;
        PositionPage *m_position_page;
        EmployeePage *m_employee_page;

        void setupComponents();
        common::BasePage* currentPage() const;
    };
}
