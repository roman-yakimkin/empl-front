//
// Created by Roman Yakimkin on 14.06.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_departmentpage.h" resolved
#include <QPointer>
#include <QMessageBox>

#include "departmentpage.h"
#include "ui_departmentpage.h"
#include "models/table/department.h"

namespace views {
    DepartmentPage::DepartmentPage(core::IDepartmentSvc* svc, QWidget *parent) : BasePage(parent), ui(new Ui::DepartmentPage), m_svc(svc) {
        ui->setupUi(this);

        setupModel();
        setupComponents();
    }

    DepartmentPage::~DepartmentPage() {
        delete ui;
    }

    void DepartmentPage::setupComponents() {
        connect(ui->btnAdd, &QPushButton::clicked, this, &DepartmentPage::onBtnAddClicked);
        connect(ui->btnDel, &QPushButton::clicked, this, &DepartmentPage::onBtnDelClicked);
        connect(ui->btnSave, &QPushButton::clicked, this, &DepartmentPage::onBtnSaveClicked);
        connect(ui->btnDelDo, &QPushButton::clicked, this, &DepartmentPage::onBtnDelDoClicked);
        connect (ui->btnIUCancel, &QPushButton::clicked, this, &DepartmentPage::onBtnCancelClicked);
        connect (ui->btnDelCancel, &QPushButton::clicked, this, &DepartmentPage::onBtnCancelClicked);

        connect(m_svc, &core::IDepartmentSvc::departmentsLoaded, this, &DepartmentPage::onDepartmentsLoaded);
        connect(m_svc, &core::IDepartmentSvc::errorOccurred, this, &DepartmentPage::onServiceError);

        connect(ui->tableView, &QTableView::doubleClicked, this, &DepartmentPage::onTableViewDblClicked);
        connect(ui->tableView->selectionModel(),&QItemSelectionModel::currentRowChanged,this,&DepartmentPage::onCurrentRowChanged);

        m_widget_right_width_max = ui->widgetRight->width();
        m_widget_right_width_min = ui->btnAdd->width() + 20;

        ui->widgetRight->setFixedWidth(m_widget_right_width_min);

        ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
        ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

        // Задаём ширину для фиксированных столбцов:
        ui->tableView->setColumnWidth(0, 50);   // ID
    }

    void DepartmentPage::setupModel() {
        m_model = new views::models::table::Department(this);
        ui->tableView->setModel(m_model);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableView->verticalHeader()->setVisible(false);

        connect(m_svc, &core::IDepartmentSvc::departmentUpdated,this, &DepartmentPage::departmentUpdated);
        connect(m_svc, &core::IDepartmentSvc::departmentCreated,this, &DepartmentPage::departmentCreated);
        connect(m_svc, &core::IDepartmentSvc::departmentDeleted, this, &DepartmentPage::departmentDeleted);
    }

    void DepartmentPage::reloadData() {
        loadData();
    }

    void DepartmentPage::onBtnAddClicked() {
        m_current = {};
        setCurrentWidget(ui->pgInsUpd);
    }

    void DepartmentPage::onBtnDelClicked() {
        setCurrentWidget(ui->pgDel);
    }

    void DepartmentPage::onDepartmentsLoaded(const QVector<model::Department> &items) {
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

    void DepartmentPage::onServiceError(const QString &code, const QString &message) {
        qWarning() << "Ошибка сервиса [" << code << "]:" << message;

        QMessageBox::critical(
            this,
            tr("Ошибка"),
            tr("Не удалось выполнить операцию:\n%1").arg(message)
        );

        ui->tableView->setEnabled(true);
    }

    void DepartmentPage::loadData() {
        ui->tableView->setEnabled(false);
        m_svc->getAll();
    }

    void DepartmentPage::onBtnSaveClicked() {
        if (editMode())
            m_svc->update(cmd::department::Update(m_current.id, ui->leItemName->text()));
        else
            m_svc->create(cmd::department::Create(ui->leItemName->text()));
    }

    void DepartmentPage::onBtnDelDoClicked() {
        m_svc->del(cmd::department::Delete(m_current.id));
    }

    void DepartmentPage::onBtnCancelClicked() {
        setCurrentWidget(ui->pgBtn);
    }

    void DepartmentPage::onTableViewDblClicked(const QModelIndex& index) {
        ui->leItemName->setText(m_current.name);
        ui->tableView->setEnabled(false);
        setCurrentWidget(ui->pgInsUpd);
    }

    void DepartmentPage::onCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous) {
        updateCurrent(current);
    }

    void DepartmentPage::updateCurrent(const QModelIndex &index) {
        if (index.isValid())
            m_current = m_model->getByRow(index.row());
        else
            m_current = {};
    }

    bool DepartmentPage::editMode() const {
        return m_current.id > 0;
    }

    void DepartmentPage::departmentCreated(const model::Department &item) {
        m_model->addItem(item);
        setCurrentWidget(ui->pgBtn);
    }

    void DepartmentPage::departmentUpdated(const model::Department &item) {
        m_model->updateItem(item);
        setCurrentWidget(ui->pgBtn);
    }

    void DepartmentPage::departmentDeleted(int id) {
        m_model->deleteItem(id);
        setCurrentWidget(ui->pgBtn);
    }

    void DepartmentPage::setCurrentWidget(QWidget* current) {
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
}

