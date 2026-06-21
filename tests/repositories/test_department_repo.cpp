//
// Created by Roman Yakimkin on 21.06.2026.
//

#include <QtTest>
#include <QtTest/qtestcase.h>

#include "repositories/department.h"
#include "../mocks/mock_http_client.h"

class TestDepartmentRepo : public QObject {
    Q_OBJECT

private slots:
    // getOne
    void testGetOneSuccess();
    void testGetOneError();
    void testGetOneEndpoint();

    // getAll
    void testGetAllSuccess();
    void testGetAllError();
    void testGetAllInvalidFormat();
    void testGetAllEndpoint();

    // create
    void testCreateSuccess();
    void testCreateError();
    void testCreateEndpointAndPayload();

    // update
    void testUpdateSuccess();
    void testUpdateError();
    void testUpdateEndpointAndPayload();

    // del
    void testDeleteSuccess();
    void testDeleteError();
    void testDeleteEndpoint();

};

void TestDepartmentRepo::testGetOneSuccess() {
    auto mock_http = new MockHttpClient();

    mock_http->response_to_return = QJsonObject{
        {"id", 42},
        {"name", "Отдел АСУ"}
    };
    mock_http->error_to_return = "";

    repo::Department repo(mock_http);
    bool callback_called = false;
    model::Department result_item;
    QString result_error;

    repo.getOne(42, [&](const model::Department& item, const QString& err){
        callback_called = true;
        result_item = item;
        result_error = err;
    });

    QVERIFY(callback_called);
    QVERIFY(result_error.isEmpty());
    QCOMPARE(result_item.id, 42);
    QCOMPARE(result_item.name, QString("Отдел АСУ"));
}

void TestDepartmentRepo::testGetOneError() {
    auto mock_http = new MockHttpClient();
    mock_http->response_to_return = QJsonObject{};
    mock_http->error_to_return = "Network error";

    repo::Department repo(mock_http);

    bool callback_called = false;
    QString result_error;

    repo.getOne(42, [&](const model::Department& item, const QString& error) {
        callback_called = true;
        result_error = error;
    });

    QVERIFY(callback_called);
    QCOMPARE(result_error, QString("Network error"));
}

void TestDepartmentRepo::testGetOneEndpoint() {
    auto mock_http = new MockHttpClient();
    repo::Department repo(mock_http);

    repo.getOne(123, [](const model::Department&, const QString&) {});

    QCOMPARE(mock_http->last_endpoint, QString("/departments/123"));
    QCOMPARE(mock_http->get_call_count, 1);
}

void TestDepartmentRepo::testGetAllSuccess() {
    auto mock_http = new MockHttpClient();
    QJsonArray items;
    items.append(QJsonObject{{"id", 1}, {"name", "Отдел АСУ"}});
    items.append(QJsonObject{{"id", 2}, {"name", "Отдел кадров"}});
    mock_http->response_to_return = QJsonObject{{"items", items}};
    mock_http->error_to_return = "";

    repo::Department repo(mock_http);

    bool callback_called = false;
    QVector<model::Department> result_items;

    repo.getAll([&](const QVector<model::Department>& items, const QString& error) {
        callback_called = true;
        result_items = items;
        QVERIFY(error.isEmpty());
    });

    QVERIFY(callback_called);
    QCOMPARE(result_items.size(), 2);
    QCOMPARE(result_items[0].id, 1);
    QCOMPARE(result_items[0].name, QString("Отдел АСУ"));
    QCOMPARE(result_items[1].id, 2);
    QCOMPARE(result_items[1].name, QString("Отдел кадров"));
}

void TestDepartmentRepo::testGetAllError() {
    auto mock_http = new MockHttpClient();
    mock_http->error_to_return = "Server error";

    repo::Department repo(mock_http);

    bool callback_called = false;
    QString result_error;

    repo.getAll([&](const QVector<model::Department>&, const QString& error) {
        callback_called = true;
        result_error = error;
    });

    QVERIFY(callback_called);
    QCOMPARE(result_error, QString("Server error"));
}

void TestDepartmentRepo::testGetAllInvalidFormat() {
    auto mock_http = new MockHttpClient();
    mock_http->response_to_return = QJsonObject{{"data", "invalid"}}; // нет "items"
    mock_http->error_to_return = "";

    repo::Department repo(mock_http);

    bool callback_called = false;
    QString result_error;

    repo.getAll([&](const QVector<model::Department>&, const QString& error) {
        callback_called = true;
        result_error = error;
    });

    QVERIFY(callback_called);
    QCOMPARE(result_error, QString("Неверный формат ответа сервера"));
}

void TestDepartmentRepo::testGetAllEndpoint() {
    auto mock_http = new MockHttpClient();
    repo::Department repo(mock_http);

    repo.getAll([](const QVector<model::Department>&, const QString&) {});

    QCOMPARE(mock_http->last_endpoint, QString("/departments"));
    QCOMPARE(mock_http->get_call_count, 1);
}

void TestDepartmentRepo::testCreateSuccess() {
    auto mock_http = new MockHttpClient();
    mock_http->response_to_return = QJsonObject{{"id", 10}, {"name", "New item"}};
    mock_http->error_to_return = "";

    repo::Department repo(mock_http);

    bool callback_called = false;
    model::Department result_item;

    cmd::department::Create cmd{"New item"};
    repo.create(cmd, [&](const model::Department& item, const QString& error) {
        callback_called = true;
        result_item = item;
        QVERIFY(error.isEmpty());
    });

    QVERIFY(callback_called);
    QCOMPARE(result_item.id, 10);
    QCOMPARE(result_item.name, QString("New item"));
}

void TestDepartmentRepo::testCreateError() {
    auto mock_http = new MockHttpClient();
    mock_http->error_to_return = "Validation error";

    repo::Department repo(mock_http);

    bool callback_called = false;
    QString result_error;

    cmd::department::Create cmd{"Test"};
    repo.create(cmd, [&](const model::Department&, const QString& error) {
        callback_called = true;
        result_error = error;
    });

    QVERIFY(callback_called);
    QCOMPARE(result_error, QString("Validation error"));
}

void TestDepartmentRepo::testCreateEndpointAndPayload() {
    auto mock_http = new MockHttpClient();
    repo::Department repo(mock_http);

    cmd::department::Create cmd{"Test Item"};
    repo.create(cmd, [](const model::Department&, const QString&) {});

    QCOMPARE(mock_http->last_endpoint, QString("/departments"));
    QCOMPARE(mock_http->last_payload["name"].toString(), QString("Test Item"));
    QCOMPARE(mock_http->post_call_count, 1);
}

void TestDepartmentRepo::testUpdateSuccess() {
    auto mock_http = new MockHttpClient();
    mock_http->response_to_return = QJsonObject{{"id", 5}, {"name", "Updated"}};
    mock_http->error_to_return = "";

    repo::Department repo(mock_http);

    bool callback_called = false;
    model::Department result_item;

    cmd::department::Update cmd{5, "Updated"};
    repo.update(cmd, [&](const model::Department& item, const QString& error) {
        callback_called = true;
        result_item = item;
        QVERIFY(error.isEmpty());
    });

    QVERIFY(callback_called);
    QCOMPARE(result_item.id, 5);
    QCOMPARE(result_item.name, QString("Updated"));
}

void TestDepartmentRepo::testUpdateError() {
    auto mock_http = new MockHttpClient();
    mock_http->error_to_return = "Not found";

    repo::Department repo(mock_http);

    bool callback_called = false;
    QString result_error;

    cmd::department::Update cmd{999, "Test"};
    repo.update(cmd, [&](const model::Department&, const QString& error) {
        callback_called = true;
        result_error = error;
    });

    QVERIFY(callback_called);
    QCOMPARE(result_error, QString("Not found"));
}

void TestDepartmentRepo::testUpdateEndpointAndPayload() {
    auto mock_http = new MockHttpClient();
    repo::Department repo(mock_http);

    cmd::department::Update cmd{42, "New Name"};
    repo.update(cmd, [](const model::Department&, const QString&) {});

    QCOMPARE(mock_http->last_endpoint, QString("/departments/42"));
    QCOMPARE(mock_http->last_payload["id"].toInt(), 42);
    QCOMPARE(mock_http->last_payload["name"].toString(), QString("New Name"));
    QCOMPARE(mock_http->put_call_count, 1);
}

void TestDepartmentRepo::testDeleteSuccess() {
    auto mock_http = new MockHttpClient();
    mock_http->error_to_return = "";

    repo::Department repo(mock_http);

    bool callback_called = false;
    bool result_success = false;

    cmd::department::Delete cmd{10};
    repo.del(cmd, [&](bool success, const QString& error) {
        callback_called = true;
        result_success = success;
        QVERIFY(error.isEmpty());
    });

    QVERIFY(callback_called);
    QVERIFY(result_success);
}

void TestDepartmentRepo::testDeleteError() {
    auto mock_http = new MockHttpClient();
    mock_http->error_to_return = "Cannot delete";

    repo::Department repo(mock_http);

    bool callback_called = false;
    bool result_success = true;
    QString result_error;

    cmd::department::Delete cmd{10};
    repo.del(cmd, [&](bool success, const QString& error) {
        callback_called = true;
        result_success = success;
        result_error = error;
    });

    QVERIFY(callback_called);
    QVERIFY(!result_success);
    QCOMPARE(result_error, QString("Cannot delete"));
}

void TestDepartmentRepo::testDeleteEndpoint() {
    auto mock_http = new MockHttpClient();
    repo::Department repo(mock_http);

    cmd::department::Delete cmd{55};
    repo.del(cmd, [](bool, const QString&) {});

    QCOMPARE(mock_http->last_endpoint, QString("/departments/55"));
    QCOMPARE(mock_http->del_call_count, 1);
}

QTEST_MAIN(TestDepartmentRepo)

#include "test_department_repo.moc"
