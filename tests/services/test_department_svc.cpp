//
// Created by Roman Yakimkin on 21.06.2026.
//

#include <QtTest>
#include <QSignalSpy>
#include <QtTest/qtestcase.h>

#include "services/department.h"
#include "helpers/msg.h"
#include "../mocks/mock_department_repo.h"

class TestDepartmentSvc : public QObject {
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
};

void TestDepartmentSvc::testGetOneSuccess() {
    auto mock_repo = new MockDepartmentRepo();
    mock_repo->item_to_return = model::Department(42, "Отдел АСУ");
    mock_repo->error_to_return = "";

    svc::Department svc(mock_repo);

    QSignalSpy loaded_spy(&svc, &svc::Department::departmentLoaded);
    QSignalSpy error_spy(&svc, &svc::Department::errorOccurred);

    svc.getOne(42);

    QCOMPARE(loaded_spy.count(), 1);
    QCOMPARE(error_spy.count(), 0);

    // Проверяем, что в сигнале пришёл правильный объект
    auto args = loaded_spy.takeFirst();
    auto loaded_item = args.at(0).value<model::Department>();
    QCOMPARE(loaded_item.id, 42);
    QCOMPARE(loaded_item.name, QString("Отдел АСУ"));
}

void TestDepartmentSvc::testGetOneError() {
    auto mock_repo = new MockDepartmentRepo();
    mock_repo->error_to_return = "Not found";

    svc::Department svc(mock_repo);

    QSignalSpy loaded_spy(&svc, &svc::Department::departmentLoaded);
    QSignalSpy error_spy(&svc, &svc::Department::errorOccurred);

    svc.getOne(999);

    QCOMPARE(loaded_spy.count(), 0);
    QCOMPARE(error_spy.count(), 1);

    auto args = error_spy.takeFirst();
    auto error_code = args.at(0).value<QString>();
    auto error_msg = args.at(1).toString();

    QCOMPARE(error_code, msg::err::ERR_LOAD_DEPARTMENT);
    QCOMPARE(error_msg, QString("Not found"));
}

void TestDepartmentSvc::testGetOnePassesId() {
    auto mock_repo = new MockDepartmentRepo();
    svc::Department svc(mock_repo);

    svc.getOne(123);

    QCOMPARE(mock_repo->last_id, 123);
    QCOMPARE(mock_repo->get_one_call_count, 1);
}

void TestDepartmentSvc::testGetAllSuccess() {
    auto mock_repo = new MockDepartmentRepo();
    mock_repo->items_to_return = {
        model::Department(1, "Отдел АСУ"),
        model::Department(2, "Транспортный цех"),
        model::Department(3, "Бухгалтерия")
    };
    mock_repo->error_to_return = "";

    svc::Department svc(mock_repo);

    QSignalSpy loaded_spy(&svc, &svc::Department::departmentsLoaded);
    QSignalSpy error_spy(&svc, &svc::Department::errorOccurred);

    svc.getAll();

    QCOMPARE(loaded_spy.count(), 1);
    QCOMPARE(error_spy.count(), 0);

    auto args = loaded_spy.takeFirst();
    auto items = args.at(0).value<QVector<model::Department>>();
    QCOMPARE(items.size(), 3);
    QCOMPARE(items[0].name, QString("Отдел АСУ"));
    QCOMPARE(items[2].name, QString("Бухгалтерия"));
}

void TestDepartmentSvc::testGetAllError() {
    auto mock_repo = new MockDepartmentRepo();
    mock_repo->error_to_return = "Server error";

    svc::Department svc(mock_repo);

    QSignalSpy loaded_spy(&svc, &svc::Department::departmentsLoaded);
    QSignalSpy error_spy(&svc, &svc::Department::errorOccurred);

    svc.getAll();

    QCOMPARE(loaded_spy.count(), 0);
    QCOMPARE(error_spy.count(), 1);

    auto args = error_spy.takeFirst();
    QCOMPARE(args.at(0).value<QString>(), msg::err::ERR_LOAD_ALL_DEPARTMENTS);
    QCOMPARE(args.at(1).toString(), QString("Server error"));
}

void TestDepartmentSvc::testCreateSuccess() {
    auto mock_repo = new MockDepartmentRepo();
    mock_repo->item_to_return = model::Department(10, "Новый отдел");
    mock_repo->error_to_return = "";

    svc::Department svc(mock_repo);

    QSignalSpy created_spy(&svc, &svc::Department::departmentCreated);
    QSignalSpy error_spy(&svc, &svc::Department::errorOccurred);

    cmd::department::Create cmd{"New Item"};
    svc.create(cmd);

    QCOMPARE(created_spy.count(), 1);
    QCOMPARE(error_spy.count(), 0);

    auto args = created_spy.takeFirst();
    auto created_item = args.at(0).value<model::Department>();
    QCOMPARE(created_item.id, 10);
    QCOMPARE(created_item.name, QString("Новый отдел"));
}

void TestDepartmentSvc::testCreateError() {
    auto mock_repo = new MockDepartmentRepo();
    mock_repo->error_to_return = "Validation error";

    svc::Department svc(mock_repo);

    QSignalSpy created_spy(&svc, &svc::Department::departmentCreated);
    QSignalSpy error_spy(&svc, &svc::Department::errorOccurred);

    cmd::department::Create cmd{"Test"};
    svc.create(cmd);

    QCOMPARE(created_spy.count(), 0);
    QCOMPARE(error_spy.count(), 1);

    auto args = error_spy.takeFirst();
    QCOMPARE(args.at(0).value<QString>(), msg::err::ERR_CREATE_DEPARTMENT);
    QCOMPARE(args.at(1).toString(), QString("Validation error"));
}

void TestDepartmentSvc::testCreatePassesCommand() {
    auto mock_repo = new MockDepartmentRepo();
    svc::Department svc(mock_repo);

    cmd::department::Create cmd{"Test"};
    svc.create(cmd);

    QCOMPARE(mock_repo->last_create_cmd.name, QString("Test"));
    QCOMPARE(mock_repo->create_call_count, 1);
}

void TestDepartmentSvc::testUpdateSuccess() {
    auto mock_repo = new MockDepartmentRepo();
    mock_repo->item_to_return = model::Department(5, "Updated");
    mock_repo->error_to_return = "";

    svc::Department svc(mock_repo);

    QSignalSpy updated_spy(&svc, &svc::Department::departmentUpdated);
    QSignalSpy error_spy(&svc, &svc::Department::errorOccurred);

    cmd::department::Update cmd{5, "Updated"};
    svc.update(cmd);

    QCOMPARE(updated_spy.count(), 1);
    QCOMPARE(error_spy.count(), 0);

    auto args = updated_spy.takeFirst();
    auto updated_dept = args.at(0).value<model::Department>();
    QCOMPARE(updated_dept.id, 5);
    QCOMPARE(updated_dept.name, QString("Updated"));
}

void TestDepartmentSvc::testUpdateError() {
    auto mock_repo = new MockDepartmentRepo();
    mock_repo->error_to_return = "Not found";

    svc::Department svc(mock_repo);

    QSignalSpy updated_spy(&svc, &svc::Department::departmentUpdated);
    QSignalSpy error_spy(&svc, &svc::Department::errorOccurred);

    cmd::department::Update cmd{999, "Test"};
    svc.update(cmd);

    QCOMPARE(updated_spy.count(), 0);
    QCOMPARE(error_spy.count(), 1);

    auto args = error_spy.takeFirst();
    QCOMPARE(args.at(0).value<QString>(), msg::err::ERR_UPDATE_DEPARTMENT);
    QCOMPARE(args.at(1).toString(), QString("Not found"));
}

void TestDepartmentSvc::testUpdatePassesCommand() {
    auto mock_repo = new MockDepartmentRepo();
    svc::Department svc(mock_repo);

    cmd::department::Update cmd{42, "New Name"};
    svc.update(cmd);

    QCOMPARE(mock_repo->last_update_cmd.id, 42);
    QCOMPARE(mock_repo->last_update_cmd.name, QString("New Name"));
    QCOMPARE(mock_repo->update_call_count, 1);
}

void TestDepartmentSvc::testDeleteSuccess() {
    auto mock_repo = new MockDepartmentRepo();
    mock_repo->bool_result_to_return = true;
    mock_repo->error_to_return = "";

    svc::Department svc(mock_repo);

    QSignalSpy deleted_spy(&svc, &svc::Department::departmentDeleted);
    QSignalSpy error_spy(&svc, &svc::Department::errorOccurred);

    cmd::department::Delete cmd{10};
    svc.del(cmd);

    QCOMPARE(deleted_spy.count(), 1);
    QCOMPARE(error_spy.count(), 0);

    auto args = deleted_spy.takeFirst();
    QCOMPARE(args.at(0).toInt(), 10);  // ID удалённого отдела
}

void TestDepartmentSvc::testDeleteError() {
    auto mock_repo = new MockDepartmentRepo();
    mock_repo->bool_result_to_return = false;
    mock_repo->error_to_return = "Cannot delete";

    svc::Department svc(mock_repo);

    QSignalSpy deleted_spy(&svc, &svc::Department::departmentDeleted);
    QSignalSpy error_spy(&svc, &svc::Department::errorOccurred);

    cmd::department::Delete cmd{10};
    svc.del(cmd);

    QCOMPARE(deleted_spy.count(), 0);
    QCOMPARE(error_spy.count(), 1);

    auto args = error_spy.takeFirst();
    QCOMPARE(args.at(0).value<QString>(), msg::err::ERR_DELETE_DEPARTMENT);
    QCOMPARE(args.at(1).toString(), QString("Cannot delete"));
}

void TestDepartmentSvc::testDeletePassesCommand() {
    auto mock_repo = new MockDepartmentRepo();
    svc::Department svc(mock_repo);

    cmd::department::Delete cmd{55};
    svc.del(cmd);

    QCOMPARE(mock_repo->last_delete_cmd.id, 55);
    QCOMPARE(mock_repo->del_call_count, 1);
}

void TestDepartmentSvc::testDeleteFailureWithoutError() {
    auto mock_repo = new MockDepartmentRepo();
    mock_repo->bool_result_to_return = false;
    mock_repo->error_to_return = "";

    svc::Department svc(mock_repo);

    QSignalSpy deleted_spy(&svc, &svc::Department::departmentDeleted);
    QSignalSpy error_spy(&svc, &svc::Department::errorOccurred);

    cmd::department::Delete cmd{10};
    svc.del(cmd);

    QCOMPARE(deleted_spy.count(), 0);
    QCOMPARE(error_spy.count(), 1);

    auto args = error_spy.takeFirst();
    QCOMPARE(args.at(0).value<QString>(), msg::err::ERR_DELETE_DEPARTMENT);
}

QTEST_MAIN(TestDepartmentSvc)

#include "test_department_svc.moc"
