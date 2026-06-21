//
// Created by Roman Yakimkin on 21.06.2026.
//

#include <QtTest>
#include <QtTest/qtestcase.h>

#include "repositories/position.h"
#include "../mocks/mock_http_client.h"


class TestPositionRepo : public QObject {
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

void TestPositionRepo::testGetOneSuccess() {
    auto mock_http = new MockHttpClient();

    mock_http->response_to_return = QJsonObject{
        {"id", 42},
        {"name", "Инженер"}
    };
    mock_http->error_to_return = "";

    repo::Position repo(mock_http);
    bool callback_called = false;
    model::Position result_item;
    QString result_error;

    repo.getOne(42, [&](const model::Position& item, const QString& err){
        callback_called = true;
        result_item = item;
        result_error = err;
    });

    QVERIFY(callback_called);
    QVERIFY(result_error.isEmpty());
    QCOMPARE(result_item.id, 42);
    QCOMPARE(result_item.name, QString("Инженер"));
}

void TestPositionRepo::testGetOneError() {
    auto mock_http = new MockHttpClient();
    mock_http->response_to_return = QJsonObject{};
    mock_http->error_to_return = "Network error";

    repo::Position repo(mock_http);

    bool callback_called = false;
    QString result_error;

    repo.getOne(42, [&](const model::Position& item, const QString& error) {
        callback_called = true;
        result_error = error;
    });

    QVERIFY(callback_called);
    QCOMPARE(result_error, QString("Network error"));
}

void TestPositionRepo::testGetOneEndpoint() {
    auto mock_http = new MockHttpClient();
    repo::Position repo(mock_http);

    repo.getOne(123, [](const model::Position&, const QString&) {});

    QCOMPARE(mock_http->last_endpoint, QString("/positions/123"));
    QCOMPARE(mock_http->get_call_count, 1);
}

void TestPositionRepo::testGetAllSuccess() {
    auto mock_http = new MockHttpClient();
    QJsonArray items;
    items.append(QJsonObject{{"id", 1}, {"name", "Иванов"}});
    items.append(QJsonObject{{"id", 2}, {"name", "Петров"}});
    mock_http->response_to_return = QJsonObject{{"items", items}};
    mock_http->error_to_return = "";

    repo::Position repo(mock_http);

    bool callback_called = false;
    QVector<model::Position> result_items;

    repo.getAll([&](const QVector<model::Position>& items, const QString& error) {
        callback_called = true;
        result_items = items;
        QVERIFY(error.isEmpty());
    });

    QVERIFY(callback_called);
    QCOMPARE(result_items.size(), 2);
    QCOMPARE(result_items[0].id, 1);
    QCOMPARE(result_items[0].name, QString("Иванов"));
    QCOMPARE(result_items[1].id, 2);
    QCOMPARE(result_items[1].name, QString("Петров"));
}

void TestPositionRepo::testGetAllError() {
    auto mock_http = new MockHttpClient();
    mock_http->error_to_return = "Server error";

    repo::Position repo(mock_http);

    bool callback_called = false;
    QString result_error;

    repo.getAll([&](const QVector<model::Position>&, const QString& error) {
        callback_called = true;
        result_error = error;
    });

    QVERIFY(callback_called);
    QCOMPARE(result_error, QString("Server error"));
}

void TestPositionRepo::testGetAllInvalidFormat() {
    auto mock_http = new MockHttpClient();
    mock_http->response_to_return = QJsonObject{{"data", "invalid"}}; // нет "items"
    mock_http->error_to_return = "";

    repo::Position repo(mock_http);

    bool callback_called = false;
    QString result_error;

    repo.getAll([&](const QVector<model::Position>&, const QString& error) {
        callback_called = true;
        result_error = error;
    });

    QVERIFY(callback_called);
    QCOMPARE(result_error, QString("Неверный формат ответа сервера"));
}

void TestPositionRepo::testGetAllEndpoint() {
    auto mock_http = new MockHttpClient();
    repo::Position repo(mock_http);

    repo.getAll([](const QVector<model::Position>&, const QString&) {});

    QCOMPARE(mock_http->last_endpoint, QString("/positions"));
    QCOMPARE(mock_http->get_call_count, 1);
}

void TestPositionRepo::testCreateSuccess() {
    auto mock_http = new MockHttpClient();
    mock_http->response_to_return = QJsonObject{{"id", 10}, {"name", "New item"}};
    mock_http->error_to_return = "";

    repo::Position repo(mock_http);

    bool callback_called = false;
    model::Position result_item;

    cmd::position::Create cmd{"New item"};
    repo.create(cmd, [&](const model::Position& item, const QString& error) {
        callback_called = true;
        result_item = item;
        QVERIFY(error.isEmpty());
    });

    QVERIFY(callback_called);
    QCOMPARE(result_item.id, 10);
    QCOMPARE(result_item.name, QString("New item"));
}

void TestPositionRepo::testCreateError() {
    auto mock_http = new MockHttpClient();
    mock_http->error_to_return = "Validation error";

    repo::Position repo(mock_http);

    bool callback_called = false;
    QString result_error;

    cmd::position::Create cmd{"Test"};
    repo.create(cmd, [&](const model::Position&, const QString& error) {
        callback_called = true;
        result_error = error;
    });

    QVERIFY(callback_called);
    QCOMPARE(result_error, QString("Validation error"));
}

void TestPositionRepo::testCreateEndpointAndPayload() {
    auto mock_http = new MockHttpClient();
    repo::Position repo(mock_http);

    cmd::position::Create cmd{"Test Item"};
    repo.create(cmd, [](const model::Position&, const QString&) {});

    QCOMPARE(mock_http->last_endpoint, QString("/positions"));
    QCOMPARE(mock_http->last_payload["name"].toString(), QString("Test Item"));
    QCOMPARE(mock_http->post_call_count, 1);
}

void TestPositionRepo::testUpdateSuccess() {
    auto mock_http = new MockHttpClient();
    mock_http->response_to_return = QJsonObject{{"id", 5}, {"name", "Updated"}};
    mock_http->error_to_return = "";

    repo::Position repo(mock_http);

    bool callback_called = false;
    model::Position result_item;

    cmd::position::Update cmd{5, "Updated"};
    repo.update(cmd, [&](const model::Position& item, const QString& error) {
        callback_called = true;
        result_item = item;
        QVERIFY(error.isEmpty());
    });

    QVERIFY(callback_called);
    QCOMPARE(result_item.id, 5);
    QCOMPARE(result_item.name, QString("Updated"));
}

void TestPositionRepo::testUpdateError() {
    auto mock_http = new MockHttpClient();
    mock_http->error_to_return = "Not found";

    repo::Position repo(mock_http);

    bool callback_called = false;
    QString result_error;

    cmd::position::Update cmd{999, "Test"};
    repo.update(cmd, [&](const model::Position&, const QString& error) {
        callback_called = true;
        result_error = error;
    });

    QVERIFY(callback_called);
    QCOMPARE(result_error, QString("Not found"));
}

void TestPositionRepo::testUpdateEndpointAndPayload() {
    auto mock_http = new MockHttpClient();
    repo::Position repo(mock_http);

    cmd::position::Update cmd{42, "New Name"};
    repo.update(cmd, [](const model::Position&, const QString&) {});

    QCOMPARE(mock_http->last_endpoint, QString("/positions/42"));
    QCOMPARE(mock_http->last_payload["id"].toInt(), 42);
    QCOMPARE(mock_http->last_payload["name"].toString(), QString("New Name"));
    QCOMPARE(mock_http->put_call_count, 1);
}

void TestPositionRepo::testDeleteSuccess() {
    auto mock_http = new MockHttpClient();
    mock_http->error_to_return = "";

    repo::Position repo(mock_http);

    bool callback_called = false;
    bool result_success = false;

    cmd::position::Delete cmd{10};
    repo.del(cmd, [&](bool success, const QString& error) {
        callback_called = true;
        result_success = success;
        QVERIFY(error.isEmpty());
    });

    QVERIFY(callback_called);
    QVERIFY(result_success);
}

void TestPositionRepo::testDeleteError() {
    auto mock_http = new MockHttpClient();
    mock_http->error_to_return = "Cannot delete";

    repo::Position repo(mock_http);

    bool callback_called = false;
    bool result_success = true;
    QString result_error;

    cmd::position::Delete cmd{10};
    repo.del(cmd, [&](bool success, const QString& error) {
        callback_called = true;
        result_success = success;
        result_error = error;
    });

    QVERIFY(callback_called);
    QVERIFY(!result_success);
    QCOMPARE(result_error, QString("Cannot delete"));
}

void TestPositionRepo::testDeleteEndpoint() {
    auto mock_http = new MockHttpClient();
    repo::Position repo(mock_http);

    cmd::position::Delete cmd{55};
    repo.del(cmd, [](bool, const QString&) {});

    QCOMPARE(mock_http->last_endpoint, QString("/positions/55"));
    QCOMPARE(mock_http->del_call_count, 1);
}

QTEST_MAIN(TestPositionRepo)

#include "test_position_repo.moc"
