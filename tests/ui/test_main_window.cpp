//
// Created by Roman Yakimkin on 23.06.2026.
//
#include <QAbstractButton>
#include <QtTest>
#include <QAction>
#include <QStackedWidget>
#include <QMessageBox>
#include <QTimer>
#include <QCloseEvent>

#include "ui/mainwindow.h"
#include "ui/departmentpage.h"
#include "ui/positionpage.h"
#include "ui/employeepage.h"
#include "infrastructure/appconfig.h"

#include "../mocks/mock_department_svc.h"
#include "../mocks/mock_position_svc.h"
#include "../mocks/mock_employee_svc.h"
#include "../mocks/mock_configurable_client.h"

class MainWindowTest : public QObject {
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void test_constructor_throws_on_null_dependencies();
    void test_initial_state_loads_employee_data();
    void test_actionDepartments_switches_page();
    void test_actionPositions_switches_page();
    void test_tab_change_updates_window_title();
    void test_closeEvent_ignored_on_no();
    void test_closeEvent_accepted_on_yes();

private:
    MockDepartmentSvc* mock_dep_svc = nullptr;
    MockPositionSvc* mock_pos_svc = nullptr;
    MockEmployeeSvc* mock_emp_svc = nullptr;
    infra::AppConfig* cfg = nullptr;
    MockConfigurableClient* mock_client = nullptr;
    views::MainWindow* main_window = nullptr;
};

void MainWindowTest::init() {
    mock_dep_svc = new MockDepartmentSvc();
    mock_pos_svc = new MockPositionSvc();
    mock_emp_svc = new MockEmployeeSvc();
    cfg = new infra::AppConfig();
    mock_client = new MockConfigurableClient();

    main_window = new views::MainWindow(
        mock_dep_svc, mock_pos_svc, mock_emp_svc, cfg, mock_client
    );
    main_window->show();
    QVERIFY(QTest::qWaitForWindowActive(main_window));
}

void MainWindowTest::cleanup() {
    delete main_window;
    delete mock_dep_svc;
    delete mock_pos_svc;
    delete mock_emp_svc;
    delete cfg;
    delete mock_client;

    main_window = nullptr;
    mock_dep_svc = nullptr;
    mock_pos_svc = nullptr;
    mock_emp_svc = nullptr;
    cfg = nullptr;
    mock_client = nullptr;
}

void MainWindowTest::test_constructor_throws_on_null_dependencies() {
    MockDepartmentSvc dep;
    MockPositionSvc pos;
    MockEmployeeSvc emp;
    infra::AppConfig c;
    MockConfigurableClient cl;

    QVERIFY_THROWS_EXCEPTION(std::invalid_argument, new views::MainWindow(nullptr, &pos, &emp, &c, &cl));
    QVERIFY_THROWS_EXCEPTION(std::invalid_argument, new views::MainWindow(&dep, nullptr, &emp, &c, &cl));
    QVERIFY_THROWS_EXCEPTION(std::invalid_argument, new views::MainWindow(&dep, &pos, nullptr, &c, &cl));
    QVERIFY_THROWS_EXCEPTION(std::invalid_argument, new views::MainWindow(&dep, &pos, &emp, nullptr, &cl));
    QVERIFY_THROWS_EXCEPTION(std::invalid_argument, new views::MainWindow(&dep, &pos, &emp, &c, nullptr));
}

void MainWindowTest::test_initial_state_loads_employee_data() {
    // В setupComponents() вызывается setCurrentWidget(m_employee_page),
    // что триггерит onStackWidgetChanged -> reloadData() -> getAll()
    QVERIFY(mock_emp_svc->get_all_called);
}

void MainWindowTest::test_actionDepartments_switches_page() {
    auto action_deps = main_window->findChild<QAction*>("actionDepartments");
    QVERIFY2(action_deps != nullptr, "actionDepartments not found in MainWindow");

    auto dep_page = main_window->findChild<views::DepartmentPage*>();
    auto stack = main_window->findChild<QStackedWidget*>();

    mock_dep_svc->get_all_called = false;

    action_deps->trigger();
    qApp->processEvents();

    QCOMPARE(stack->currentWidget(), dep_page);
    QVERIFY(mock_dep_svc->get_all_called); // Проверка, что reloadData() отработал
}

void MainWindowTest::test_actionPositions_switches_page() {
    auto action_pos = main_window->findChild<QAction*>("actionPositions");
    QVERIFY2(action_pos != nullptr, "actionPositions not found in MainWindow");

    auto pos_page = main_window->findChild<views::PositionPage*>();
    auto stack = main_window->findChild<QStackedWidget*>();

    mock_pos_svc->get_all_called = false;

    action_pos->trigger();
    qApp->processEvents();

    QCOMPARE(stack->currentWidget(), pos_page);
    QVERIFY(mock_pos_svc->get_all_called);
}

void MainWindowTest::test_tab_change_updates_window_title() {
    auto action_deps = main_window->findChild<QAction*>("actionDepartments");
    action_deps->trigger();
    qApp->processEvents();

    // В mainwindow.cpp: setWindowTitle("Учет сотрудников - " + current->windowTitle());
    QVERIFY(main_window->windowTitle().contains(QStringLiteral("Учет сотрудников")));
    // Предполагаем, что windowTitle() у DepartmentPage содержит "Отделы"
    QVERIFY(main_window->windowTitle().contains(QStringLiteral("Отделы")));
}

void MainWindowTest::test_closeEvent_ignored_on_no() {
    // Эмулируем нажатие "Нет" в модальном окне через 200 мс
    QTimer::singleShot(200, []() {
        QWidget* modal = QApplication::activeModalWidget();
        if (auto msg_box = qobject_cast<QMessageBox*>(modal)) {
            msg_box->button(QMessageBox::No)->click();
        }
    });

    QCloseEvent close_event;
    QApplication::sendEvent(main_window, &close_event);
    qApp->processEvents();

    QVERIFY(!close_event.isAccepted());
    QVERIFY(main_window->isVisible());
}

void MainWindowTest::test_closeEvent_accepted_on_yes() {
    // Эмулируем нажатие "Да" в модальном окне через 200 мс
    QTimer::singleShot(200, []() {
        QWidget* modal = QApplication::activeModalWidget();
        if (auto msg_box = qobject_cast<QMessageBox*>(modal)) {
            msg_box->button(QMessageBox::Yes)->click();
        }
    });

    QCloseEvent close_event;
    QApplication::sendEvent(main_window, &close_event);
    qApp->processEvents();

    QVERIFY(close_event.isAccepted());
}

QTEST_MAIN(MainWindowTest)
#include "test_main_window.moc"