//
// Created by Roman Yakimkin on 22.06.2026.
//

#include <QtTest>
#include <QSignalSpy>
#include <QtTest/qtestcase.h>

#include "services/position.h"
#include "helpers/msg.h"
#include "../mocks/mock_position_repo.h"

class TestPositionSvc : public QObject {
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

void TestPositionSvc::testGetOneSuccess() {
    auto mock_repo = new MockPositionRepo();
    mock_repo->item_to_return = model::Position(42, "Инженер");
    mock_repo->error_to_return = "";

    svc::Position svc(mock_repo);

    QSignalSpy loaded_spy(&svc, &svc::Position::positionLoaded);
    QSignalSpy error_spy(&svc, &svc::Position::errorOccurred);

    svc.getOne(42);

    QCOMPARE(loaded_spy.count(), 1);
    QCOMPARE(error_spy.count(), 0);

    // Проверяем, что в сигнале пришёл правильный объект
    auto args = loaded_spy.takeFirst();
    auto loaded_item = args.at(0).value<model::Position>();
    QCOMPARE(loaded_item.id, 42);
    QCOMPARE(loaded_item.name, QString("Инженер"));
}

void TestPositionSvc::testGetOneError() {
    auto mock_repo = new MockPositionRepo();
    mock_repo->error_to_return = "Not found";

    svc::Position svc(mock_repo);

    QSignalSpy loaded_spy(&svc, &svc::Position::positionLoaded);
    QSignalSpy error_spy(&svc, &svc::Position::errorOccurred);

    svc.getOne(999);

    QCOMPARE(loaded_spy.count(), 0);
    QCOMPARE(error_spy.count(), 1);

    auto args = error_spy.takeFirst();
    auto error_code = args.at(0).value<QString>();
    auto error_msg = args.at(1).toString();

    QCOMPARE(error_code, msg::err::ERR_LOAD_POSITION);
    QCOMPARE(error_msg, QString("Not found"));
}

void TestPositionSvc::testGetOnePassesId() {
    auto mock_repo = new MockPositionRepo();
    svc::Position svc(mock_repo);

    svc.getOne(123);

    QCOMPARE(mock_repo->last_id, 123);
    QCOMPARE(mock_repo->get_one_call_count, 1);
}

void TestPositionSvc::testGetAllSuccess() {
    auto mock_repo = new MockPositionRepo();
    mock_repo->items_to_return = {
        model::Position(1, "Директор"),
        model::Position(2, "Бухгалтер"),
        model::Position(3, "Водитель")
    };
    mock_repo->error_to_return = "";

    svc::Position svc(mock_repo);

    QSignalSpy loaded_spy(&svc, &svc::Position::positionsLoaded);
    QSignalSpy error_spy(&svc, &svc::Position::errorOccurred);

    svc.getAll();

    QCOMPARE(loaded_spy.count(), 1);
    QCOMPARE(error_spy.count(), 0);

    auto args = loaded_spy.takeFirst();
    auto items = args.at(0).value<QVector<model::Position>>();
    QCOMPARE(items.size(), 3);
    QCOMPARE(items[0].name, QString("Директор"));
    QCOMPARE(items[2].name, QString("Водитель"));
}

void TestPositionSvc::testGetAllError() {
    auto mock_repo = new MockPositionRepo();
    mock_repo->error_to_return = "Server error";

    svc::Position svc(mock_repo);

    QSignalSpy loaded_spy(&svc, &svc::Position::positionsLoaded);
    QSignalSpy error_spy(&svc, &svc::Position::errorOccurred);

    svc.getAll();

    QCOMPARE(loaded_spy.count(), 0);
    QCOMPARE(error_spy.count(), 1);

    auto args = error_spy.takeFirst();
    QCOMPARE(args.at(0).value<QString>(), msg::err::ERR_LOAD_ALL_POSITIONS);
    QCOMPARE(args.at(1).toString(), QString("Server error"));
}

void TestPositionSvc::testCreateSuccess() {
    auto mock_repo = new MockPositionRepo();
    mock_repo->item_to_return = model::Position(10, "Новая должность");
    mock_repo->error_to_return = "";

    svc::Position svc(mock_repo);

    QSignalSpy created_spy(&svc, &svc::Position::positionCreated);
    QSignalSpy error_spy(&svc, &svc::Position::errorOccurred);

    cmd::position::Create cmd{"New Item"};
    svc.create(cmd);

    QCOMPARE(created_spy.count(), 1);
    QCOMPARE(error_spy.count(), 0);

    auto args = created_spy.takeFirst();
    auto created_item = args.at(0).value<model::Position>();
    QCOMPARE(created_item.id, 10);
    QCOMPARE(created_item.name, QString("Новая должность"));
}

void TestPositionSvc::testCreateError() {
    auto mock_repo = new MockPositionRepo();
    mock_repo->error_to_return = "Validation error";

    svc::Position svc(mock_repo);

    QSignalSpy created_spy(&svc, &svc::Position::positionCreated);
    QSignalSpy error_spy(&svc, &svc::Position::errorOccurred);

    cmd::position::Create cmd{"Test"};
    svc.create(cmd);

    QCOMPARE(created_spy.count(), 0);
    QCOMPARE(error_spy.count(), 1);

    auto args = error_spy.takeFirst();
    QCOMPARE(args.at(0).value<QString>(), msg::err::ERR_CREATE_POSITION);
    QCOMPARE(args.at(1).toString(), QString("Validation error"));
}

void TestPositionSvc::testCreatePassesCommand() {
    auto mock_repo = new MockPositionRepo();
    svc::Position svc(mock_repo);

    cmd::position::Create cmd{"Test"};
    svc.create(cmd);

    QCOMPARE(mock_repo->last_create_cmd.name, QString("Test"));
    QCOMPARE(mock_repo->create_call_count, 1);
}

void TestPositionSvc::testUpdateSuccess() {
    auto mock_repo = new MockPositionRepo();
    mock_repo->item_to_return = model::Position(5, "Updated");
    mock_repo->error_to_return = "";

    svc::Position svc(mock_repo);

    QSignalSpy updated_spy(&svc, &svc::Position::positionUpdated);
    QSignalSpy error_spy(&svc, &svc::Position::errorOccurred);

    cmd::position::Update cmd{5, "Updated"};
    svc.update(cmd);

    QCOMPARE(updated_spy.count(), 1);
    QCOMPARE(error_spy.count(), 0);

    auto args = updated_spy.takeFirst();
    auto updated_dept = args.at(0).value<model::Position>();
    QCOMPARE(updated_dept.id, 5);
    QCOMPARE(updated_dept.name, QString("Updated"));
}

void TestPositionSvc::testUpdateError() {
    auto mock_repo = new MockPositionRepo();
    mock_repo->error_to_return = "Not found";

    svc::Position svc(mock_repo);

    QSignalSpy updated_spy(&svc, &svc::Position::positionUpdated);
    QSignalSpy error_spy(&svc, &svc::Position::errorOccurred);

    cmd::position::Update cmd{999, "Test"};
    svc.update(cmd);

    QCOMPARE(updated_spy.count(), 0);
    QCOMPARE(error_spy.count(), 1);

    auto args = error_spy.takeFirst();
    QCOMPARE(args.at(0).value<QString>(), msg::err::ERR_UPDATE_POSITION);
    QCOMPARE(args.at(1).toString(), QString("Not found"));
}

void TestPositionSvc::testUpdatePassesCommand() {
    auto mock_repo = new MockPositionRepo();
    svc::Position svc(mock_repo);

    cmd::position::Update cmd{42, "New Name"};
    svc.update(cmd);

    QCOMPARE(mock_repo->last_update_cmd.id, 42);
    QCOMPARE(mock_repo->last_update_cmd.name, QString("New Name"));
    QCOMPARE(mock_repo->update_call_count, 1);
}

void TestPositionSvc::testDeleteSuccess() {
    auto mock_repo = new MockPositionRepo();
    mock_repo->bool_result_to_return = true;
    mock_repo->error_to_return = "";

    svc::Position svc(mock_repo);

    QSignalSpy deleted_spy(&svc, &svc::Position::positionDeleted);
    QSignalSpy error_spy(&svc, &svc::Position::errorOccurred);

    cmd::position::Delete cmd{10};
    svc.del(cmd);

    QCOMPARE(deleted_spy.count(), 1);
    QCOMPARE(error_spy.count(), 0);

    auto args = deleted_spy.takeFirst();
    QCOMPARE(args.at(0).toInt(), 10);  // ID удалённого отдела
}

void TestPositionSvc::testDeleteError() {
    auto mock_repo = new MockPositionRepo();
    mock_repo->bool_result_to_return = false;
    mock_repo->error_to_return = "Cannot delete";

    svc::Position svc(mock_repo);

    QSignalSpy deleted_spy(&svc, &svc::Position::positionDeleted);
    QSignalSpy error_spy(&svc, &svc::Position::errorOccurred);

    cmd::position::Delete cmd{10};
    svc.del(cmd);

    QCOMPARE(deleted_spy.count(), 0);
    QCOMPARE(error_spy.count(), 1);

    auto args = error_spy.takeFirst();
    QCOMPARE(args.at(0).value<QString>(), msg::err::ERR_DELETE_POSITION);
    QCOMPARE(args.at(1).toString(), QString("Cannot delete"));
}

void TestPositionSvc::testDeletePassesCommand() {
    auto mock_repo = new MockPositionRepo();
    svc::Position svc(mock_repo);

    cmd::position::Delete cmd{55};
    svc.del(cmd);

    QCOMPARE(mock_repo->last_delete_cmd.id, 55);
    QCOMPARE(mock_repo->del_call_count, 1);
}

void TestPositionSvc::testDeleteFailureWithoutError() {
    auto mock_repo = new MockPositionRepo();
    mock_repo->bool_result_to_return = false;
    mock_repo->error_to_return = "";

    svc::Position svc(mock_repo);

    QSignalSpy deleted_spy(&svc, &svc::Position::positionDeleted);
    QSignalSpy error_spy(&svc, &svc::Position::errorOccurred);

    cmd::position::Delete cmd{10};
    svc.del(cmd);

    QCOMPARE(deleted_spy.count(), 0);
    QCOMPARE(error_spy.count(), 1);

    auto args = error_spy.takeFirst();
    QCOMPARE(args.at(0).value<QString>(), msg::err::ERR_DELETE_POSITION);
}

QTEST_MAIN(TestPositionSvc)

#include "test_position_svc.moc"
