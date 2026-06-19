//
// Created by Roman Yakimkin on 14.06.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_empoyeepage.h" resolved
#include <QPointer>
#include <QMessageBox>
#include <QInputDialog>
#include <QTimer>

#include "employeepage.h"
#include "ui_employeepage.h"
#include "core/cmd/employee.h"
#include "models/table/employee.h"

namespace views {
    EmployeePage::EmployeePage(core::IEmployeeSvc* svc, core::IDepartmentSvc* dep_svc, core::IPositionSvc *pos_svc, QWidget *parent)
        : BasePage(parent), ui(new Ui::EmployeePage), m_svc(svc), m_dep_svc(dep_svc), m_pos_svc(pos_svc) {
        ui->setupUi(this);

        setupModel();
        setupComponents();
    }

    EmployeePage::~EmployeePage() {
        delete ui;
    }

    void EmployeePage::setupComponents() {
        connect(ui->btnAdd, &QPushButton::clicked, this, &EmployeePage::onBtnAddClicked);
        connect(ui->btnDel, &QPushButton::clicked, this, &EmployeePage::onBtnDelClicked);
        connect(ui->btnSave, &QPushButton::clicked, this, &EmployeePage::onBtnSaveClicked);
        connect(ui->btnDelDo, &QPushButton::clicked, this, &EmployeePage::onBtnDelDoClicked);
        connect (ui->btnIUCancel, &QPushButton::clicked, this, &EmployeePage::onBtnCancelClicked);
        connect (ui->btnDelCancel, &QPushButton::clicked, this, &EmployeePage::onBtnCancelClicked);

        connect(m_svc, &core::IEmployeeSvc::employeesLoaded, this, &EmployeePage::onEmployeesLoaded);
        connect(m_svc, &core::IEmployeeSvc::employeeLoaded, this, &EmployeePage::onEmployeeLoaded);
        connect(m_svc, &core::IEmployeeSvc::errorOccurred, this, &EmployeePage::onServiceError);

        connect(ui->tableView, &QTableView::doubleClicked, this, &EmployeePage::onTableViewDblClicked);
        connect(ui->tableView->selectionModel(),&QItemSelectionModel::currentRowChanged,this,&EmployeePage::onCurrentRowChanged);

        connect(m_dep_svc, &core::IDepartmentSvc::departmentsLoaded,this, &EmployeePage::onDepartmentsLoaded);
        connect(m_dep_svc, &core::IDepartmentSvc::departmentCreated,this, &EmployeePage::onDepartmentCreated);
        connect(m_dep_svc, &core::IDepartmentSvc::departmentDeleted,this, &EmployeePage::onDepartmentDeleted);
        connect(ui->btnAddDepartment, &QPushButton::clicked,this, &EmployeePage::onBtnAddDepartmentClicked);

        connect(m_pos_svc, &core::IPositionSvc::positionsLoaded,this, &EmployeePage::onPositionsLoaded);
        connect(m_pos_svc, &core::IPositionSvc::positionCreated,this, &EmployeePage::onPositionCreated);
        connect(m_pos_svc, &core::IPositionSvc::positionDeleted,this, &EmployeePage::onPositionDeleted);
        connect(ui->btnAddPosition, &QPushButton::clicked,this, &EmployeePage::onBtnAddPositionClicked);

        m_widget_right_width_max = ui->widgetRight->width();
        m_widget_right_width_min = ui->btnAdd->width() + 20;

        ui->widgetRight->setFixedWidth(m_widget_right_width_min);

        ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
        ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
        ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
        ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
        ui->tableView->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);

        // Задаём ширину для фиксированных столбцов:
        ui->tableView->setColumnWidth(0, 50);   // ID
        ui->tableView->setColumnWidth(4, 150);  // Дата создания
        ui->tableView->setColumnWidth(5, 150);  // Дата изменения
    }

    void EmployeePage::setupModel() {
        m_model = new views::models::table::Employee(this);
        ui->tableView->setModel(m_model);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableView->verticalHeader()->setVisible(false);

        connect(m_svc, &core::IEmployeeSvc::employeeUpdated,this, &EmployeePage::employeeUpdated);
        connect(m_svc, &core::IEmployeeSvc::employeeCreated,this, &EmployeePage::employeeCreated);
        connect(m_svc, &core::IEmployeeSvc::employeeDeleted, this, &EmployeePage::employeeDeleted);
    }

    void EmployeePage::reloadData() {
        loadData();
    }

    void EmployeePage::onBtnAddClicked() {
        m_current = {};
        setCurrentWidget(ui->pgInsUpd);
    }

    void EmployeePage::onBtnDelClicked() {
        setCurrentWidget(ui->pgDel);
    }

    void EmployeePage::onEmployeesLoaded(const QVector<model::Employee> &items) {
        m_model->setItems(items);

        ui->tableView->setEnabled(true);

        if (!items.isEmpty()) {
            QModelIndex first_index = m_model->index(0, 0);
            ui->tableView->selectionModel()->select(first_index, QItemSelectionModel::Select | QItemSelectionModel::Clear);
            ui->tableView->setCurrentIndex(first_index);
        } else {
            ui->tableView->clearSelection();
        }
    }

    void EmployeePage::onEmployeeLoaded(const model::Employee &item) {
        m_model->updateItem(item);
        setCurrentWidget(ui->pgBtn);
    }

    void EmployeePage::onServiceError(const QString &code, const QString &message) {
        qWarning() << "Ошибка сервиса [" << code << "]:" << message;

        QMessageBox::critical(
            this,
            tr("Ошибка"),
            tr("Не удалось выполнить операцию:\n%1").arg(message)
        );

        ui->tableView->setEnabled(true);
    }

    void EmployeePage::loadData() {
        ui->tableView->setEnabled(false);
        m_svc->getAll();
        loadDepartments();
        loadPositions();
    }

    void EmployeePage::loadDepartments() {
        m_dep_svc->getAll();
    }

    void EmployeePage::loadPositions() {
        m_pos_svc->getAll();
    }

    void EmployeePage::onBtnSaveClicked() {
        int dept_id = getSelectedDepartmentId();
        if (dept_id <= 0) {
            QMessageBox::warning(this, tr("Ошибка"), tr("Выберите отдел"));
            return;
        }
        int pos_id = getSelectedPositionId();
        if (pos_id <= 0) {
            QMessageBox::warning(this, tr("Ошибка"), tr("Выберите должность"));
            return;
        }

        QString name = ui->leItemName->text().trimmed();

        if (editMode())
            m_svc->update(cmd::employee::Update(
                m_current.id,
                name,
                dept_id,
                pos_id
            ));
        else
            m_svc->create(cmd::employee::Create(
                name,
                dept_id,
                pos_id
            ));
    }

    void EmployeePage::onBtnDelDoClicked() {
        m_svc->del(cmd::employee::Delete(m_current.id));
    }

    void EmployeePage::onBtnCancelClicked() {
        setCurrentWidget(ui->pgBtn);
    }

    void EmployeePage::onTableViewDblClicked(const QModelIndex& index) {
        ui->leItemName->setText(m_current.name);
        selectDepartmentInComboBox(m_current.department.id);
        selectPositionInComboBox(m_current.position.id);
        ui->tableView->setEnabled(false);
        setCurrentWidget(ui->pgInsUpd);
    }

    void EmployeePage::onCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous) {
        updateCurrent(current);
    }

    void EmployeePage::updateCurrent(const QModelIndex &index) {
        if (index.isValid())
            m_current = m_model->getByRow(index.row());
        else
            m_current = {};
    }

    bool EmployeePage::editMode() const {
        return m_current.id > 0;
    }

    void EmployeePage::employeeCreated(const model::Employee &item) {
        m_model->addItem(item);
        QTimer::singleShot(100, this, [this, item]() {
           m_svc->getOne(item.id);
        });
    }

    void EmployeePage::employeeUpdated(const model::Employee &item) {
        QTimer::singleShot(100, this, [this, item]() {
            m_svc->getOne(item.id);
        });
    }

    void EmployeePage::employeeDeleted(int id) {
        m_model->deleteItem(id);
        setCurrentWidget(ui->pgBtn);
    }

    void EmployeePage::setCurrentWidget(QWidget* current) {
        if (!current) return;

        ui->stackedWidget->setCurrentWidget(current);

        if (current == ui->pgBtn) {
            ui->widgetRight->setFixedWidth(m_widget_right_width_min);
            ui->tableView->setDisabled(false);
            ui->leItemName->clear();

            updateCurrent(ui->tableView->currentIndex());
        }
        else {
            ui->widgetRight->setFixedWidth(m_widget_right_width_max);
            ui->tableView->setDisabled(true);
        }

        if (current == ui->pgDel) {
            QString caption = ui->lbDelConfirm->text();
            caption.replace("%1", "<strong>" + m_current.name + "</strong>");
            ui->lbDelConfirm->setText(caption);
        }
    }

    void EmployeePage::onDepartmentsLoaded(const QVector<model::Department> &items) {
        m_departments = items;
        fillDepartmentsComboBox();
    }

    void EmployeePage::fillDepartmentsComboBox() {
        ui->cbDepartments->clear();
        ui->cbDepartments->addItem("- Выберите отдел -");

        for (const auto& item : m_departments) {
            ui->cbDepartments->addItem(item.name, item.id);
        }
    }

    int EmployeePage::getSelectedDepartmentId() const {
        return ui->cbDepartments->currentData().toInt();
    }

    void EmployeePage::selectDepartmentInComboBox(int dept_id) {
        for (int i = 0; i < ui->cbDepartments->count(); ++i) {
            if (ui->cbDepartments->itemData(i).toInt() == dept_id) {
                ui->cbDepartments->setCurrentIndex(i);

                return;
            }
        }
    }

    void EmployeePage::onBtnAddDepartmentClicked() {
        QInputDialog dlg(this);
        dlg.setWindowTitle(tr("Новый отдел"));
        dlg.setLabelText(tr("Название отдела:"));
        dlg.setInputMode(QInputDialog::TextInput);
        dlg.setTextValue("");
        dlg.resize(280, 150);

        if (dlg.exec() == QDialog::Accepted) {
            if (QString name = dlg.textValue().trimmed(); !name.isEmpty())
                m_dep_svc->create(cmd::department::Create(name));
        }
    }

    void EmployeePage::onDepartmentCreated(const model::Department &item) {
        m_departments.append(item);
        fillDepartmentsComboBox();

        selectDepartmentInComboBox(item.id);
    }

    void EmployeePage::onDepartmentDeleted(int id) {
        m_departments.erase(
        std::ranges::remove_if(m_departments,
            [id](const model::Department& d) { return d.id == id; }).begin(),
        m_departments.end()
        );

        fillDepartmentsComboBox();
    }

    void EmployeePage::onPositionsLoaded(const QVector<model::Position> &items) {
        m_positions = items;
        fillPositionsComboBox();
    }

    void EmployeePage::fillPositionsComboBox() {
        ui->cbPositions->clear();
        ui->cbPositions->addItem("- Выберите должность -");

        for (const auto& item : m_positions) {
            ui->cbPositions->addItem(item.name, item.id);
        }
    }

    int EmployeePage::getSelectedPositionId() const {
        return ui->cbPositions->currentData().toInt();
    }

    void EmployeePage::selectPositionInComboBox(int pos_id) {
        for (int i = 0; i < ui->cbPositions->count(); ++i) {
            if (ui->cbPositions->itemData(i).toInt() == pos_id) {
                ui->cbPositions->setCurrentIndex(i);

                return;
            }
        }
    }

    void EmployeePage::onBtnAddPositionClicked() {
        QInputDialog dlg(this);
        dlg.setWindowTitle(tr("Новая должность"));
        dlg.setLabelText(tr("Название должности:"));
        dlg.setInputMode(QInputDialog::TextInput);
        dlg.setTextValue("");
        dlg.resize(280, 150);

        if (dlg.exec() == QDialog::Accepted) {
            if (QString name = dlg.textValue().trimmed(); !name.isEmpty())
                m_pos_svc->create(cmd::position::Create(name));
        }
    }

    void EmployeePage::onPositionCreated(const model::Position &item) {
        m_positions.append(item);
        fillPositionsComboBox();

        selectPositionInComboBox(item.id);
    }

    void EmployeePage::onPositionDeleted(int id) {
        m_positions.erase(
        std::ranges::remove_if(m_positions,
            [id](const model::Position& p) { return p.id == id; }).begin(),
        m_positions.end()
        );

        fillPositionsComboBox();
    }

}

