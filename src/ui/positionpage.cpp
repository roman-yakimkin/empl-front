//
// Created by Roman Yakimkin on 14.06.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_departmentpage.h" resolved
#include <QPointer>
#include <QMessageBox>

#include "positionpage.h"

#include <qabstractitemview.h>
#include <QHeaderView>

#include "ui_positionpage.h"
#include "models/table/position.h"

namespace views {
    PositionPage::PositionPage(core::IPositionSvc* svc, QWidget *parent) : BasePage(parent), ui(new Ui::PositionPage), m_svc(svc) {
        ui->setupUi(this);

        setUpModel();
        setUpComponents();
    }

    PositionPage::~PositionPage() {
        delete ui;
    }

    void PositionPage::setUpComponents() {
        connect(ui->btnAdd, &QPushButton::clicked, this, &PositionPage::onBtnAddClicked);
        connect(ui->btnDel, &QPushButton::clicked, this, &PositionPage::onBtnDelClicked);
        connect(ui->btnSave, &QPushButton::clicked, this, &PositionPage::onBtnSaveClicked);
        connect(ui->btnDelDo, &QPushButton::clicked, this, &PositionPage::onBtnDelDoClicked);
        connect (ui->btnIUCancel, &QPushButton::clicked, this, &PositionPage::onBtnCancelClicked);
        connect (ui->btnDelCancel, &QPushButton::clicked, this, &PositionPage::onBtnCancelClicked);

        connect(m_svc, &core::IPositionSvc::positionsLoaded, this, &PositionPage::onPositionsLoaded);
        connect(m_svc, &core::IPositionSvc::errorOccurred, this, &PositionPage::onServiceError);

        connect(ui->tableView, &QTableView::doubleClicked, this, &PositionPage::onTableViewDblClicked);
        connect(ui->tableView->selectionModel(),&QItemSelectionModel::currentRowChanged,this,&PositionPage::onCurrentRowChanged);

        m_widget_right_width_max = ui->widgetRight->width();
        m_widget_right_width_min = ui->btnAdd->width() + 20;

        ui->widgetRight->setFixedWidth(m_widget_right_width_min);

        ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
        ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

        // Задаём ширину для фиксированных столбцов:
        ui->tableView->setColumnWidth(0, 50);   // ID
    }

    void PositionPage::setUpModel() {
        m_model = new views::models::table::Position(this);
        ui->tableView->setModel(m_model);
        ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ui->tableView->verticalHeader()->setVisible(false);

        connect(m_svc, &core::IPositionSvc::positionUpdated,this, &PositionPage::positionUpdated);
        connect(m_svc, &core::IPositionSvc::positionCreated,this, &PositionPage::positionCreated);
        connect(m_svc, &core::IPositionSvc::positionDeleted, this, &PositionPage::positionDeleted);
    }

    void PositionPage::reloadData() {
        loadData();
    }

    void PositionPage::onBtnAddClicked() {
        m_current = {};
        setCurrentWidget(ui->pgInsUpd);
    }

    void PositionPage::onBtnDelClicked() {
        setCurrentWidget(ui->pgDel);
    }

    void PositionPage::onPositionsLoaded(const QVector<model::Position> &items) {
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

    void PositionPage::onServiceError(const QString &code, const QString &message) {
        qWarning() << "Ошибка сервиса [" << code << "]:" << message;

        QMessageBox::critical(
            this,
            tr("Ошибка"),
            tr("Не удалось выполнить операцию:\n%1").arg(message)
        );

        ui->tableView->setEnabled(true);
    }

    void PositionPage::loadData() {
        ui->tableView->setEnabled(false);
        m_svc->getAll();
    }

    void PositionPage::onBtnSaveClicked() {
        if (editMode())
            m_svc->update(cmd::position::Update(m_current.id, ui->leItemName->text()));
        else
            m_svc->create(cmd::position::Create(ui->leItemName->text()));
    }

    void PositionPage::onBtnDelDoClicked() {
        m_svc->del(cmd::position::Delete(m_current.id));
    }

    void PositionPage::onBtnCancelClicked() {
        setCurrentWidget(ui->pgBtn);
    }

    void PositionPage::onTableViewDblClicked(const QModelIndex& index) {
        ui->leItemName->setText(m_current.name);
        ui->tableView->setEnabled(false);
        setCurrentWidget(ui->pgInsUpd);
    }

    void PositionPage::onCurrentRowChanged(const QModelIndex &current, const QModelIndex &previous) {
        updateCurrent(current);
    }

    void PositionPage::updateCurrent(const QModelIndex &index) {
        if (index.isValid())
            m_current = m_model->getByRow(index.row());
        else
            m_current = {};
    }

    bool PositionPage::editMode() const {
        return m_current.id > 0;
    }

    void PositionPage::positionCreated(const model::Position &item) {
        m_model->addItem(item);
        setCurrentWidget(ui->pgBtn);
    }

    void PositionPage::positionUpdated(const model::Position &item) {
        m_model->updateItem(item);
        setCurrentWidget(ui->pgBtn);
    }

    void PositionPage::positionDeleted(int id) {
        m_model->deleteItem(id);
        setCurrentWidget(ui->pgBtn);
    }

    void PositionPage::setCurrentWidget(QWidget* current) {
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

