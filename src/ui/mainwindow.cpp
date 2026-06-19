//
// Created by Roman Yakimkin on 11.06.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include <QMainWindow>
#include <QToolBar>
#include <QToolButton>
#include <QLabel>
#include <QMenu>
#include <QCloseEvent>
#include <QDialog>

#include "ui_mainwindow.h"
#include "mainwindow.h"

#include <QMessageBox>

#include "settings.h"

namespace views {
    MainWindow::MainWindow(
        core::IDepartmentSvc* dep_svc,
        core::IPositionSvc* pos_svc,
        core::IEmployeeSvc* empl_svc,
        infra::AppConfig* cfg,
        core::IConfigurableClient* client,
        QWidget *parent
    ) : QMainWindow(parent) {
        if (!dep_svc) throw std::invalid_argument("department service cannot be null");
        if (!pos_svc) throw std::invalid_argument("position service cannot be null");
        if (!empl_svc) throw std::invalid_argument("employee service cannot be null");
        if (!cfg) throw std::invalid_argument("config cannot be null");

        m_dep_svc = dep_svc;
        m_pos_svc = pos_svc;
        m_empl_svc = empl_svc;

        m_cfg = cfg;
        m_client = client;

        ui = new Ui::MainWindow;
        ui->setupUi(this);

        m_stack_widget = new QStackedWidget(this);
        m_department_page = new DepartmentPage(m_dep_svc, this);
        m_position_page = new PositionPage(m_pos_svc, this);
        m_employee_page = new EmployeePage(m_empl_svc, m_dep_svc, m_pos_svc, this);

        setupComponents();
    }

    MainWindow::~MainWindow() {
        delete ui;
    }

    void MainWindow::setupComponents() {
        setCentralWidget(m_stack_widget);

        connect(m_stack_widget, &QStackedWidget::currentChanged, this, &MainWindow::onStackWidgetChanged);

        connect(ui->actionDepartments, &QAction::triggered, this, &MainWindow::onActionDepartmentsTriggered);
        connect(ui->actionPositions, &QAction::triggered, this, &MainWindow::onActionPositionsTriggered);
        connect(ui->actionEmployees, &QAction::triggered, this, &MainWindow::onActionEmployeesTriggered);
        connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::onActionSettingsTriggered);
        connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);

        m_stack_widget->addWidget(m_department_page);
        m_stack_widget->addWidget(m_position_page);
        m_stack_widget->addWidget(m_employee_page);

        m_stack_widget->setCurrentWidget(m_employee_page);
    }

    void MainWindow::closeEvent(QCloseEvent *event) {
        auto reply = QMessageBox::question(
            this,
            tr("Выход"),
            tr("Вы действительно хотите выйти?"),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
        );

        if (reply == QMessageBox::Yes)
            event->accept();
        else
            event->ignore();
    }

    void MainWindow::onActionDepartmentsTriggered() {
        m_stack_widget->setCurrentWidget(m_department_page);
    }

    void MainWindow::onActionPositionsTriggered() {
        m_stack_widget->setCurrentWidget(m_position_page);
    }

    void MainWindow::onActionEmployeesTriggered() {
        m_stack_widget->setCurrentWidget(m_employee_page);
    }

    void MainWindow::onActionSettingsTriggered() {
        if (Settings dlg(m_cfg, this); dlg.exec() == QDialog::Accepted) {
            m_client->setBaseUrl(m_cfg->apiBaseUrl());
            m_client->setPort(m_cfg->apiPort());
            m_client->setTransferTimeout(m_cfg->apiTimeout() * 1000);
        }
    }

    void MainWindow::onStackWidgetChanged(int index) {
        auto current = currentPage();
        if (!current) return;

        current->reloadData();
        setWindowTitle("Учет сотрудников - " + current->windowTitle());
    }

    common::BasePage* MainWindow::currentPage() const {
        return qobject_cast<common::BasePage*>(m_stack_widget->currentWidget());
    }
}
