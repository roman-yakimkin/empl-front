//
// Created by Roman Yakimkin on 21.06.2026.
//

#include <QtTest>
#include <QtTest/qtestcase.h>
#include <QJsonArray>
#include <QDateTime>

#include "repositories/employee.h"
#include "../mocks/mock_http_client.h"

class TestEmployeeRepo : public QObject {
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

private:
    // Вспомогательный метод для создания JSON сотрудника
    QJsonObject makeEmployeeJson(int id, const QString& name,
                                 int dept_id, const QString& dept_name,
                                 int pos_id, const QString& pos_name);
};

QJsonObject TestEmployeeRepo::makeEmployeeJson(int id, const QString &name,
    int dept_id, const QString &dept_name, int pos_id, const QString &pos_name) {

    QJsonObject dept_json{{"id", dept_id}, {"name", dept_name}};
    QJsonObject pos_json{{"id", pos_id}, {"name", pos_name}};

    return QJsonObject{
            {"id", id},
            {"name", name},
            {"department", dept_json},
            {"position", pos_json},
            {"created_at", QDateTime::currentDateTime().toString(Qt::ISODate)},
            {"updated_at", QDateTime::currentDateTime().toString(Qt::ISODate)}
    };
}

void TestEmployeeRepo::testGetOneSuccess() {
    auto mock_http = new MockHttpClient();
    mock_http->response_to_return = makeEmployeeJson(42, "Иван Иванов", 1, "Отдел АСУ", 5, "Программист");
    mock_http->error_to_return = "";

    repo::Employee repo(mock_http);

    bool callback_called = false;
    model::Employee result_item;
    QString result_error;

    repo.getOne(42, [&](const model::Employee& emp, const QString& error) {
        callback_called = true;
        result_item = emp;
        result_error = error;
    });

    QVERIFY(callback_called);
    QVERIFY(result_error.isEmpty());
    QCOMPARE(result_item.id, 42);
    QCOMPARE(result_item.name, QString("Иван Иванов"));
    QCOMPARE(result_item.department.id, 1);
    QCOMPARE(result_item.department.name, QString("Отдел АСУ"));
    QCOMPARE(result_item.position.id, 5);
    QCOMPARE(result_item.position.name, QString("Программист"));
}

void TestEmployeeRepo::testGetOneError() {
    auto mock_http = new MockHttpClient();
    mock_http->error_to_return = "Not found";

    repo::Employee repo(mock_http);

    bool callback_called = false;
    QString result_error;

    repo.getOne(999, [&](const model::Employee&, const QString& error) {
        callback_called = true;
        result_error = error;
    });

    QVERIFY(callback_called);
    QCOMPARE(result_error, QString("Not found"));
}

void TestEmployeeRepo::testGetOneEndpoint() {
    auto mock_http = new MockHttpClient();
    repo::Employee repo(mock_http);

    repo.getOne(123, [](const model::Employee&, const QString&) {});

    QCOMPARE(mock_http->last_endpoint, QString("/employees/123"));
    QCOMPARE(mock_http->get_call_count, 1);
}

void TestEmployeeRepo::testGetAllSuccess() {
    auto mock_http = new MockHttpClient();

    QJsonArray items;
    items.append(makeEmployeeJson(1, "Иван Иванов", 1, "Отдел АСУ", 5, "Программист"));
    items.append(makeEmployeeJson(2, "Пётр Петров", 2, "Транспортный отдел", 7, "Водитель"));
    items.append(makeEmployeeJson(3, "Степан Сидоров", 1, "Отдел АСУ", 6, "Ведущий инженер"));

    mock_http->response_to_return = QJsonObject{{"items", items}};
    mock_http->error_to_return = "";

    repo::Employee repo(mock_http);

    bool callback_called = false;
    QVector<model::Employee> result_items;

    repo.getAll([&](const QVector<model::Employee>& items, const QString& error) {
        callback_called = true;
        result_items = items;
        QVERIFY(error.isEmpty());
    });

    QVERIFY(callback_called);
    QCOMPARE(result_items.size(), 3);

    // Проверяем первого
    QCOMPARE(result_items[0].id, 1);
    QCOMPARE(result_items[0].name, QString("Иван Иванов"));
    QCOMPARE(result_items[0].department.name, QString("Отдел АСУ"));

    // Проверяем второго
    QCOMPARE(result_items[1].id, 2);
    QCOMPARE(result_items[1].department.id, 2);
    QCOMPARE(result_items[1].position.name, QString("Водитель"));

    // Проверяем третьего
    QCOMPARE(result_items[2].id, 3);
    QCOMPARE(result_items[2].position.name, QString("Ведущий инженер"));
}

void TestEmployeeRepo::testGetAllError() {
    auto mock_http = new MockHttpClient();
    mock_http->error_to_return = "Server error";

    repo::Employee repo(mock_http);

    bool callback_called = false;
    QString result_error;

    repo.getAll([&](const QVector<model::Employee>&, const QString& error) {
        callback_called = true;
        result_error = error;
    });

    QVERIFY(callback_called);
    QCOMPARE(result_error, QString("Server error"));
}

void TestEmployeeRepo::testGetAllInvalidFormat() {
    auto mock_http = new MockHttpClient();
    mock_http->response_to_return = QJsonObject{{"data", "invalid"}}; // нет "items"
    mock_http->error_to_return = "";

    repo::Employee repo(mock_http);

    bool callback_called = false;
    QString result_error;

    repo.getAll([&](const QVector<model::Employee>&, const QString& error) {
        callback_called = true;
        result_error = error;
    });

    QVERIFY(callback_called);
    QCOMPARE(result_error, QString("Неверный формат ответа сервера"));
}

void TestEmployeeRepo::testGetAllEndpoint() {
    auto mock_http = new MockHttpClient();
    repo::Employee repo(mock_http);

    repo.getAll([](const QVector<model::Employee>&, const QString&) {});

    QCOMPARE(mock_http->last_endpoint, QString("/employees"));
    QCOMPARE(mock_http->get_call_count, 1);
}

void TestEmployeeRepo::testCreateSuccess() {
    auto mock_http = new MockHttpClient();
    mock_http->response_to_return = makeEmployeeJson(10, "Новый Сотрудник", 1, "Отдел АСУ", 5, "Программист");
    mock_http->error_to_return = "";

    repo::Employee repo(mock_http);

    bool callback_called = false;
    model::Employee result_emp;

    cmd::employee::Create cmd{"Новый Сотрудник", 1, 5};
    repo.create(cmd, [&](const model::Employee& emp, const QString& error) {
        callback_called = true;
        result_emp = emp;
        QVERIFY(error.isEmpty());
    });

    QVERIFY(callback_called);
    QCOMPARE(result_emp.id, 10);
    QCOMPARE(result_emp.name, QString("Новый Сотрудник"));
    QCOMPARE(result_emp.department.id, 1);
    QCOMPARE(result_emp.position.id, 5);
}

void TestEmployeeRepo::testCreateError() {
    auto mock_http = new MockHttpClient();
    mock_http->error_to_return = "Validation error";

    repo::Employee repo(mock_http);

    bool callback_called = false;
    QString result_error;

    cmd::employee::Create cmd{"Test", 1, 5};
    repo.create(cmd, [&](const model::Employee&, const QString& error) {
        callback_called = true;
        result_error = error;
    });

    QVERIFY(callback_called);
    QCOMPARE(result_error, QString("Validation error"));
}

void TestEmployeeRepo::testCreateEndpointAndPayload() {
    auto mock_http = new MockHttpClient();
    repo::Employee repo(mock_http);

    cmd::employee::Create cmd{"Тестовый", 42, 17};
    repo.create(cmd, [](const model::Employee&, const QString&) {});

    QCOMPARE(mock_http->last_endpoint, QString("/employees"));
    QCOMPARE(mock_http->last_payload["name"].toString(), QString("Тестовый"));
    QCOMPARE(mock_http->last_payload["department_id"].toInt(), 42);
    QCOMPARE(mock_http->last_payload["position_id"].toInt(), 17);
    QCOMPARE(mock_http->post_call_count, 1);
}

void TestEmployeeRepo::testUpdateSuccess() {
    auto mock_http = new MockHttpClient();
    mock_http->response_to_return = makeEmployeeJson(5, "Обновлённый", 2, "Отдел кадров", 8, "Ведущий инженер");
    mock_http->error_to_return = "";

    repo::Employee repo(mock_http);

    bool callback_called = false;
    model::Employee result_emp;

    cmd::employee::Update cmd{5, "Обновлённый", 2, 8};
    repo.update(cmd, [&](const model::Employee& emp, const QString& error) {
        callback_called = true;
        result_emp = emp;
        QVERIFY(error.isEmpty());
    });

    QVERIFY(callback_called);
    QCOMPARE(result_emp.id, 5);
    QCOMPARE(result_emp.name, QString("Обновлённый"));
    QCOMPARE(result_emp.department.id, 2);
    QCOMPARE(result_emp.position.id, 8);
}

void TestEmployeeRepo::testUpdateError() {
    auto mock_http = new MockHttpClient();
    mock_http->error_to_return = "Not found";

    repo::Employee repo(mock_http);

    bool callback_called = false;
    QString result_error;

    cmd::employee::Update cmd{999, "Test", 1, 1};
    repo.update(cmd, [&](const model::Employee&, const QString& error) {
        callback_called = true;
        result_error = error;
    });

    QVERIFY(callback_called);
    QCOMPARE(result_error, QString("Not found"));
}

void TestEmployeeRepo::testUpdateEndpointAndPayload() {
    auto mock_http = new MockHttpClient();
    repo::Employee repo(mock_http);

    cmd::employee::Update cmd{42, "Новое Имя", 7, 23};
    repo.update(cmd, [](const model::Employee&, const QString&) {});

    QCOMPARE(mock_http->last_endpoint, QString("/employees/42"));
    QCOMPARE(mock_http->last_payload["id"].toInt(), 42);
    QCOMPARE(mock_http->last_payload["name"].toString(), QString("Новое Имя"));
    QCOMPARE(mock_http->last_payload["department_id"].toInt(), 7);
    QCOMPARE(mock_http->last_payload["position_id"].toInt(), 23);
    QCOMPARE(mock_http->put_call_count, 1);
}

void TestEmployeeRepo::testDeleteSuccess() {
    auto mock_http = new MockHttpClient();
    mock_http->error_to_return = "";

    repo::Employee repo(mock_http);

    bool callback_called = false;
    bool result_success = false;

    cmd::employee::Delete cmd{10};
    repo.del(cmd, [&](bool success, const QString& error) {
        callback_called = true;
        result_success = success;
        QVERIFY(error.isEmpty());
    });

    QVERIFY(callback_called);
    QVERIFY(result_success);
}

void TestEmployeeRepo::testDeleteError() {
    auto mock_http = new MockHttpClient();
    mock_http->error_to_return = "Cannot delete";

    repo::Employee repo(mock_http);

    bool callback_called = false;
    bool result_success = true;
    QString result_error;

    cmd::employee::Delete cmd{10};
    repo.del(cmd, [&](bool success, const QString& error) {
        callback_called = true;
        result_success = success;
        result_error = error;
    });

    QVERIFY(callback_called);
    QVERIFY(!result_success);
    QCOMPARE(result_error, QString("Cannot delete"));
}

void TestEmployeeRepo::testDeleteEndpoint() {
    auto mock_http = new MockHttpClient();
    repo::Employee repo(mock_http);

    cmd::employee::Delete cmd{55};
    repo.del(cmd, [](bool, const QString&) {});

    QCOMPARE(mock_http->last_endpoint, QString("/employees/55"));
    QCOMPARE(mock_http->del_call_count, 1);
}

QTEST_MAIN(TestEmployeeRepo)

#include "test_employee_repo.moc"



