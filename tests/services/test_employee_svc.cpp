//
// Created by Roman Yakimkin on 22.06.2026.
//

#include <QtTest>
#include <QObject>
#include <QDateTime>
#include <QSignalSpy>
#include <qt6/QtTest/qtestcase.h>

#include "services/employee.h"
#include "core/models/employee.h"
#include "helpers/msg.h"
#include "../mocks/mock_employee_repo.h"

class TestEmployeeSvc : public QObject {
    Q_OBJECT
private slots:
    // getOne
 void testGetOneSuccess();
    void testGetOneError();
    void testGetOnePassesId();

    // getAll
    void testGetAllSuccess();
    void testGetAllError();

    // create
    void testCreateSuccess();
    void testCreateError();
    void testCreatePassesCommand();

    // update
    void testUpdateSuccess();
    void testUpdateError();
    void testUpdatePassesCommand();

    // del
    void testDeleteSuccess();
    void testDeleteError();
    void testDeletePassesCommand();
    void testDeleteFailureWithoutError();

private:
    model::Employee make_test_employee(int id, const QString& name,
                                int dept_id, const QString& dept_name,
                                int pos_id, const QString& pos_name);
};

model::Employee TestEmployeeSvc::make_test_employee(int id, const QString& name,
                                                    int dept_id, const QString& dept_name,
                                                    int pos_id, const QString& pos_name)
{
    model::Employee emp;

    emp.id = id;
    emp.name = name;
    emp.department.id = dept_id;
    emp.department.name = dept_name;
    emp.position.id = pos_id;
    emp.position.name = pos_name;
    emp.created_at = QDateTime::currentDateTime();
    emp.updated_at = QDateTime::currentDateTime();

    return emp;
}

void TestEmployeeSvc::testGetOneSuccess() {
    auto mock_repo = new MockEmployeeRepo();
    mock_repo->emp_to_return = make_test_employee(42, "Иван Иванов", 1, "Отдел АСУ", 5, "Программист");
    mock_repo->error_to_return = "";

    svc::Employee svc(mock_repo);

    QSignalSpy loaded_spy(&svc, &svc::Employee::employeeLoaded);
    QSignalSpy error_spy(&svc, &svc::Employee::errorOccurred);

    svc.getOne(42);

    QCOMPARE(loaded_spy.count(), 1);
    QCOMPARE(error_spy.count(), 0);

    auto args = loaded_spy.takeFirst();
    auto loaded_emp = args.at(0).value<model::Employee>();
    QCOMPARE(loaded_emp.id, 42);
    QCOMPARE(loaded_emp.name, QString("Иван Иванов"));
    QCOMPARE(loaded_emp.department.id, 1);
    QCOMPARE(loaded_emp.department.name, QString("Отдел АСУ"));
    QCOMPARE(loaded_emp.position.id, 5);
    QCOMPARE(loaded_emp.position.name, QString("Программист"));
}

void TestEmployeeSvc::testGetOneError() {
    auto mock_repo = new MockEmployeeRepo();
    mock_repo->error_to_return = "Not found";

    svc::Employee svc(mock_repo);

    QSignalSpy loaded_spy(&svc, &svc::Employee::employeeLoaded);
    QSignalSpy error_spy(&svc, &svc::Employee::errorOccurred);

    svc.getOne(999);

    QCOMPARE(loaded_spy.count(), 0);
    QCOMPARE(error_spy.count(), 1);

    auto args = error_spy.takeFirst();
    auto error_code = args.at(0).value<QString>();
    auto error_msg = args.at(1).toString();

    QCOMPARE(error_code, msg::err::ERR_LOAD_EMPLOYEE);
    QCOMPARE(error_msg, QString("Not found"));
}

void TestEmployeeSvc::testGetOnePassesId() {
    auto mock_repo = new MockEmployeeRepo();
    svc::Employee svc(mock_repo);

    svc.getOne(123);

    QCOMPARE(mock_repo->last_id, 123);
    QCOMPARE(mock_repo->get_one_call_count, 1);
}

void TestEmployeeSvc::testGetAllSuccess() {
    auto mock_repo = new MockEmployeeRepo();
    mock_repo->emps_to_return = {
        make_test_employee(1, "Иван Иванов", 1, "Отдел АСУ", 5, "Программист"),
        make_test_employee(2, "Пётр Петров", 2, "Отдел кадров", 7, "Инженер"),
        make_test_employee(3, "Сергей Сидоров", 1, "Отдел АСУ", 6, "Старший программист")
    };
    mock_repo->error_to_return = "";

    svc::Employee svc(mock_repo);

    QSignalSpy loaded_spy(&svc, &svc::Employee::employeesLoaded);
    QSignalSpy error_spy(&svc, &svc::Employee::errorOccurred);

    svc.getAll();

    QCOMPARE(loaded_spy.count(), 1);
    QCOMPARE(error_spy.count(), 0);

    auto args = loaded_spy.takeFirst();
    auto emps = args.at(0).value<QVector<model::Employee>>();
    QCOMPARE(emps.size(), 3);
    QCOMPARE(emps[0].name, QString("Иван Иванов"));
    QCOMPARE(emps[0].department.name, QString("Отдел АСУ"));
    QCOMPARE(emps[1].name, QString("Пётр Петров"));
    QCOMPARE(emps[1].position.name, QString("Инженер"));
    QCOMPARE(emps[2].name, QString("Сергей Сидоров"));
    QCOMPARE(emps[2].position.name, QString("Старший программист"));
}

void TestEmployeeSvc::testGetAllError() {
    auto mock_repo = new MockEmployeeRepo();
    mock_repo->error_to_return = "Server error";

    svc::Employee svc(mock_repo);

    QSignalSpy loaded_spy(&svc, &svc::Employee::employeesLoaded);
    QSignalSpy error_spy(&svc, &svc::Employee::errorOccurred);

    svc.getAll();

    QCOMPARE(loaded_spy.count(), 0);
    QCOMPARE(error_spy.count(), 1);

    auto args = error_spy.takeFirst();
    QCOMPARE(args.at(0).value<QString>(), msg::err::ERR_LOAD_ALL_EMPLOYEES);
    QCOMPARE(args.at(1).toString(), QString("Server error"));
}

void TestEmployeeSvc::testCreateSuccess() {
    auto mock_repo = new MockEmployeeRepo();
    mock_repo->emp_to_return = make_test_employee(10, "Новый Сотрудник", 1, "Отдел АСУ", 5, "Программист");
    mock_repo->error_to_return = "";

    svc::Employee svc(mock_repo);

    QSignalSpy created_spy(&svc, &svc::Employee::employeeCreated);
    QSignalSpy error_spy(&svc, &svc::Employee::errorOccurred);

    cmd::employee::Create cmd{"Новый Сотрудник", 1, 5};
    svc.create(cmd);

    QCOMPARE(created_spy.count(), 1);
    QCOMPARE(error_spy.count(), 0);

    auto args = created_spy.takeFirst();
    auto created_emp = args.at(0).value<model::Employee>();
    QCOMPARE(created_emp.id, 10);
    QCOMPARE(created_emp.name, QString("Новый Сотрудник"));
    QCOMPARE(created_emp.department.id, 1);
    QCOMPARE(created_emp.position.id, 5);
}

void TestEmployeeSvc::testCreateError() {
    auto mock_repo = new MockEmployeeRepo();
    mock_repo->error_to_return = "Validation error";

    svc::Employee svc(mock_repo);

    QSignalSpy created_spy(&svc, &svc::Employee::employeeCreated);
    QSignalSpy error_spy(&svc, &svc::Employee::errorOccurred);

    cmd::employee::Create cmd{"Test", 1, 5};
    svc.create(cmd);

    QCOMPARE(created_spy.count(), 0);
    QCOMPARE(error_spy.count(), 1);

    auto args = error_spy.takeFirst();
    QCOMPARE(args.at(0).value<QString>(), msg::err::ERR_CREATE_EMPLOYEE);
    QCOMPARE(args.at(1).toString(), QString("Validation error"));
}

void TestEmployeeSvc::testCreatePassesCommand() {
    auto mock_repo = new MockEmployeeRepo();
    svc::Employee svc(mock_repo);

    cmd::employee::Create cmd{"Тестовый", 42, 17};
    svc.create(cmd);

    QCOMPARE(mock_repo->last_create_cmd.name, QString("Тестовый"));
    QCOMPARE(mock_repo->last_create_cmd.department_id, 42);
    QCOMPARE(mock_repo->last_create_cmd.position_id, 17);
    QCOMPARE(mock_repo->create_call_count, 1);
}

void TestEmployeeSvc::testUpdateSuccess() {
    auto mock_repo = new MockEmployeeRepo();
    mock_repo->emp_to_return = make_test_employee(5, "Обновлённый", 2, "Отдел кадров", 8, "Инженер");
    mock_repo->error_to_return = "";

    svc::Employee svc(mock_repo);

    QSignalSpy updated_spy(&svc, &svc::Employee::employeeUpdated);
    QSignalSpy error_spy(&svc, &svc::Employee::errorOccurred);

    cmd::employee::Update cmd{5, "Обновлённый", 2, 8};
    svc.update(cmd);

    QCOMPARE(updated_spy.count(), 1);
    QCOMPARE(error_spy.count(), 0);

    auto args = updated_spy.takeFirst();
    auto updated_emp = args.at(0).value<model::Employee>();
    QCOMPARE(updated_emp.id, 5);
    QCOMPARE(updated_emp.name, QString("Обновлённый"));
    QCOMPARE(updated_emp.department.id, 2);
    QCOMPARE(updated_emp.position.id, 8);
}

void TestEmployeeSvc::testUpdateError() {
    auto mock_repo = new MockEmployeeRepo();
    mock_repo->error_to_return = "Not found";

    svc::Employee svc(mock_repo);

    QSignalSpy updated_spy(&svc, &svc::Employee::employeeUpdated);
    QSignalSpy error_spy(&svc, &svc::Employee::errorOccurred);

    cmd::employee::Update cmd{999, "Test", 1, 1};
    svc.update(cmd);

    QCOMPARE(updated_spy.count(), 0);
    QCOMPARE(error_spy.count(), 1);

    auto args = error_spy.takeFirst();
    QCOMPARE(args.at(0).value<QString>(), msg::err::ERR_UPDATE_EMPLOYEE);
    QCOMPARE(args.at(1).toString(), QString("Not found"));
}

void TestEmployeeSvc::testUpdatePassesCommand() {
    auto mock_repo = new MockEmployeeRepo();
    svc::Employee svc(mock_repo);

    cmd::employee::Update cmd{42, "Новое Имя", 7, 23};
    svc.update(cmd);

    QCOMPARE(mock_repo->last_update_cmd.id, 42);
    QCOMPARE(mock_repo->last_update_cmd.name, QString("Новое Имя"));
    QCOMPARE(mock_repo->last_update_cmd.department_id, 7);
    QCOMPARE(mock_repo->last_update_cmd.position_id, 23);
    QCOMPARE(mock_repo->update_call_count, 1);
}

void TestEmployeeSvc::testDeleteSuccess() {
    auto mock_repo = new MockEmployeeRepo();
    mock_repo->bool_result_to_return = true;
    mock_repo->error_to_return = "";

    svc::Employee svc(mock_repo);

    QSignalSpy deleted_spy(&svc, &svc::Employee::employeeDeleted);
    QSignalSpy error_spy(&svc, &svc::Employee::errorOccurred);

    cmd::employee::Delete cmd{10};
    svc.del(cmd);

    QCOMPARE(deleted_spy.count(), 1);
    QCOMPARE(error_spy.count(), 0);

    auto args = deleted_spy.takeFirst();
    QCOMPARE(args.at(0).toInt(), 10);
}

void TestEmployeeSvc::testDeleteError() {
    auto mock_repo = new MockEmployeeRepo();
    mock_repo->bool_result_to_return = false;
    mock_repo->error_to_return = "Cannot delete";

    svc::Employee svc(mock_repo);

    QSignalSpy deleted_spy(&svc, &svc::Employee::employeeDeleted);
    QSignalSpy error_spy(&svc, &svc::Employee::errorOccurred);

    cmd::employee::Delete cmd{10};
    svc.del(cmd);

    QCOMPARE(deleted_spy.count(), 0);
    QCOMPARE(error_spy.count(), 1);

    auto args = error_spy.takeFirst();
    QCOMPARE(args.at(0).value<QString>(), msg::err::ERR_DELETE_EMPLOYEE);
    QCOMPARE(args.at(1).toString(), QString("Cannot delete"));
}

void TestEmployeeSvc::testDeletePassesCommand() {
    auto mock_repo = new MockEmployeeRepo();
    svc::Employee svc(mock_repo);

    cmd::employee::Delete cmd{55};
    svc.del(cmd);

    QCOMPARE(mock_repo->last_delete_cmd.id, 55);
    QCOMPARE(mock_repo->del_call_count, 1);
}

void TestEmployeeSvc::testDeleteFailureWithoutError() {
    auto mock_repo = new MockEmployeeRepo();
    mock_repo->bool_result_to_return = false;
    mock_repo->error_to_return = "";

    svc::Employee svc(mock_repo);

    QSignalSpy deleted_spy(&svc, &svc::Employee::employeeDeleted);
    QSignalSpy error_spy(&svc, &svc::Employee::errorOccurred);

    cmd::employee::Delete cmd{10};
    svc.del(cmd);

    QCOMPARE(deleted_spy.count(), 0);
    QCOMPARE(error_spy.count(), 1);

    auto args = error_spy.takeFirst();
    QCOMPARE(args.at(0).value<QString>(), msg::err::ERR_DELETE_EMPLOYEE);
}

QTEST_MAIN(TestEmployeeSvc)

#include "test_employee_svc.moc"