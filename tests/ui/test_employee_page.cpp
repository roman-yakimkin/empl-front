//
// Created by Roman Yakimkin on 23.06.2026.
//

#include <QtTest>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QTableView>
#include <QStackedWidget>
#include <QDateTime>

#include "ui/employeepage.h"
#include "ui/models/table/employee.h"
#include "../mocks/mock_employee_svc.h"
#include "../mocks/mock_department_svc.h"
#include "../mocks/mock_position_svc.h"

class EmployeePageTest : public QObject {
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void test_addButton_switchesToInsertPage();
    void test_cancelButton_returnsToMainPage();
    void test_reloadData_callsGetAllOnAllServices();
    void test_employeesLoaded_populatesModel();
    void test_departmentsLoaded_populatesComboBox();
    void test_tableViewDblClick_entersEditMode();
    void test_saveButton_callsCreate_inInsertMode();

private:
    MockEmployeeSvc* mock_emp_svc = nullptr;
    MockDepartmentSvc* mock_dep_svc = nullptr;
    MockPositionSvc* mock_pos_svc = nullptr;
    views::EmployeePage* page = nullptr;
};

void EmployeePageTest::init() {
    mock_emp_svc = new MockEmployeeSvc();
    mock_dep_svc = new MockDepartmentSvc();
    mock_pos_svc = new MockPositionSvc();

    page = new views::EmployeePage(mock_emp_svc, mock_dep_svc, mock_pos_svc);
    page->show();
    QVERIFY(QTest::qWaitForWindowActive(page));

    // Эмулируем загрузку справочников, чтобы комбобоксы были не пустыми при старте
    mock_dep_svc->emitDepartmentsLoaded({{1, "Отдел АСУ"}, {2, "Бухгалтерия"}});
    mock_pos_svc->emitPositionsLoaded({{1, "Программист"}, {2, "Бухгалтер"}});
    qApp->processEvents();
}

void EmployeePageTest::cleanup() {
    delete page;
    delete mock_emp_svc;
    delete mock_dep_svc;
    delete mock_pos_svc;
    page = nullptr;
    mock_emp_svc = nullptr;
    mock_dep_svc = nullptr;
    mock_pos_svc = nullptr;
}

void EmployeePageTest::test_addButton_switchesToInsertPage() {
    auto btn_add = page->findChild<QPushButton*>("btnAdd");
    QVERIFY2(btn_add != nullptr, "btnAdd not found");

    QTest::mouseClick(btn_add, Qt::LeftButton);

    auto stacked_widget = page->findChild<QStackedWidget*>("stackedWidget");
    auto pg_ins_upd = page->findChild<QWidget*>("pgInsUpd");
    auto table_view = page->findChild<QTableView*>("tableView");

    QCOMPARE(stacked_widget->currentWidget(), pg_ins_upd);
    QVERIFY(!table_view->isEnabled());
}

void EmployeePageTest::test_cancelButton_returnsToMainPage() {
    auto btn_add = page->findChild<QPushButton*>("btnAdd");
    QTest::mouseClick(btn_add, Qt::LeftButton);

    auto btn_cancel = page->findChild<QPushButton*>("btnIUCancel");
    QTest::mouseClick(btn_cancel, Qt::LeftButton);

    auto stacked_widget = page->findChild<QStackedWidget*>("stackedWidget");
    auto pg_btn = page->findChild<QWidget*>("pgBtn");
    auto table_view = page->findChild<QTableView*>("tableView");

    QCOMPARE(stacked_widget->currentWidget(), pg_btn);
    QVERIFY(table_view->isEnabled());
}

void EmployeePageTest::test_reloadData_callsGetAllOnAllServices() {
    mock_emp_svc->get_all_called = false;
    mock_dep_svc->get_all_called = false;
    mock_pos_svc->get_all_called = false;

    page->reloadData();

    QVERIFY(mock_emp_svc->get_all_called);
    QVERIFY(mock_dep_svc->get_all_called);
    QVERIFY(mock_pos_svc->get_all_called);
}

void EmployeePageTest::test_employeesLoaded_populatesModel() {
    model::Department dep = {1, "IT"};
    model::Position pos = {1, "Developer"};
    // Используем агрегатную инициализацию для model::Employee
    model::Employee emp = {1, "Иванов И.И.", dep, pos, QDateTime::currentDateTime(), QDateTime::currentDateTime()};

    QVector<model::Employee> test_items = {emp};
    mock_emp_svc->emitEmployeesLoaded(test_items);
    qApp->processEvents();

    auto table_view = page->findChild<QTableView*>("tableView");
    auto table_model = qobject_cast<views::models::table::Employee*>(table_view->model());

    QCOMPARE(table_model->rowCount(QModelIndex()), 1);
    QCOMPARE(table_model->data(table_model->index(0, 1), Qt::DisplayRole).toString(), QStringLiteral("Иванов И.И."));
}

void EmployeePageTest::test_departmentsLoaded_populatesComboBox() {
    // Справочники уже загружены в init(), проверяем комбобокс
    auto cb_department = page->findChild<QComboBox*>("cbDepartments");
    QVERIFY2(cb_department != nullptr, "cbDepartments not found. Check objectName in .ui file.");

    // В init() мы отправили 2 департамента, плюс, там была еще одна позиция "Выберите отдел".
    // Проверяем, что они в комбобоксе.

    QCOMPARE(cb_department->count(), 3);
    QCOMPARE(cb_department->itemText(1), QStringLiteral("Отдел АСУ"));
}

void EmployeePageTest::test_tableViewDblClick_entersEditMode() {
    model::Department dep = {1, "Отдел АСУ"};
    model::Position pos = {1, "Программист"};
    model::Employee emp = {1, "Петров П.П.", dep, pos, QDateTime(), QDateTime()};

    mock_emp_svc->emitEmployeesLoaded({emp});
    qApp->processEvents();

    auto table_view = page->findChild<QTableView*>("tableView");
    QModelIndex index = table_view->model()->index(0, 0);

    // Прямой emit сигнала, чтобы избежать проблем с фокусом и координатами
    emit table_view->doubleClicked(index);
    qApp->processEvents();

    auto stacked_widget = page->findChild<QStackedWidget*>("stackedWidget");
    auto pg_ins_upd = page->findChild<QWidget*>("pgInsUpd");

    QCOMPARE(stacked_widget->currentWidget(), pg_ins_upd);
}

void EmployeePageTest::test_saveButton_callsCreate_inInsertMode() {
    auto btn_add = page->findChild<QPushButton*>("btnAdd");
    QTest::mouseClick(btn_add, Qt::LeftButton);

    auto le_item_name = page->findChild<QLineEdit*>("leItemName");
    le_item_name->setText(QStringLiteral("Сидоров С.С."));

    auto cb_department = page->findChild<QComboBox*>("cbDepartments");
    cb_department->setCurrentIndex(1); // Выбираем "Отдел АСУ"

    auto cb_position = page->findChild<QComboBox*>("cbPositions");
    cb_position->setCurrentIndex(1); // Выбираем "Программист"

    auto btn_save = page->findChild<QPushButton*>("btnSave");
    mock_emp_svc->create_called = false;
    QTest::mouseClick(btn_save, Qt::LeftButton);

    QVERIFY(mock_emp_svc->create_called);
}

QTEST_MAIN(EmployeePageTest)
#include "test_employee_page.moc"