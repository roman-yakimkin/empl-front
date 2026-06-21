//
// Created by Roman Yakimkin on 20.06.2026.
//

#include <QtTest>
#include <QtTest/qtestcase.h>

#include "core/models/employee.h"

class TestEmployeeModel : public QObject {
    Q_OBJECT

private slots:
    void testDefaultConstructor();

    void testFromJson();
    void testToJson();
    void testRoundTrip();

    void testFromJsonEdgeCases_data();
    void testFromJsonEdgeCases();
};

void TestEmployeeModel::testDefaultConstructor() {
    model::Employee item;
    QCOMPARE(item.id, 0);
    QVERIFY(item.name.isEmpty());
    QCOMPARE(item.department.id, 0);
    QVERIFY(item.department.name.isEmpty());
    QCOMPARE(item.position.id, 0);
    QVERIFY(item.position.name.isEmpty());
}

void TestEmployeeModel::testFromJson() {
    model::Employee item(42, "Иванов", {10, "Руководство"}, {20, "Директор"});

    QJsonObject json = item.toJson();
    QJsonObject dep_json = model::Department(10, "Руководство").toJson();
    QJsonObject pos_json = model::Position({20, "Директор"}).toJson();

    QCOMPARE(json["id"].toInt(), 42);
    QCOMPARE(json["name"].toString(), QString("Иванов"));
    QCOMPARE(json["department"], model::Department(10, "Руководство").toJson());
    QCOMPARE(json["position"], model::Position({20, "Директор"}).toJson());

    QCOMPARE(json.size(), 6);
}

void TestEmployeeModel::testToJson() {
    QJsonObject json, json_dep, json_pos;

    json_dep["id"] = 10;
    json_dep["name"] = "Плановый отдел";

    json_pos["id"] = 20;
    json_pos["name"] = "Экономист";

    json["id"] = 100;
    json["name"] = "Иванов";
    json["department"] = json_dep;
    json["position"] = json_pos;

    model::Employee item = model::Employee::fromJson(json);
    QCOMPARE(item.id, 100);
    QCOMPARE(item.name, QString("Иванов"));
    QCOMPARE(item.department.id, 10);
    QCOMPARE(item.department.name, QString("Плановый отдел"));
    QCOMPARE(item.position.id, 20);
    QCOMPARE(item.position.name, QString("Экономист"));
}

void TestEmployeeModel::testRoundTrip() {
    model::Employee original(101, "Петров", {10, "Стройцех"}, {20, "Каменщик"});

    QJsonObject json = original.toJson();
    model::Employee restored = model::Employee::fromJson(json);

    QCOMPARE(restored.id, 101);
    QCOMPARE(restored.name, QString("Петров"));
    QCOMPARE(restored.department.id, 10);
    QCOMPARE(restored.department.name, QString("Стройцех"));
    QCOMPARE(restored.position.id, 20);
    QCOMPARE(restored.position.name, QString("Каменщик"));
}

void TestEmployeeModel::testFromJsonEdgeCases_data()
{
    QTest::addColumn<QJsonObject>("inputJson");
    QTest::addColumn<int>("expectedId");
    QTest::addColumn<QString>("expectedName");

    // Нормальный случай
    QTest::newRow("normal")
        << (QJsonObject{{"id", 1}, {"name", "Test"}})
        << 1 << QString("Test");

    // Отсутствует id - QJsonValue::toInt() вернёт 0
    QTest::newRow("missing_id")
        << (QJsonObject{{"name", "Test"}})
        << 0 << QString("Test");

    // Отсутствует name - QJsonValue::toString() вернёт пустую строку
    QTest::newRow("missing_name")
        << (QJsonObject{{"id", 5}})
        << 5 << QString("");

    // id передан как строка - toInt() вернёт 0
    QTest::newRow("id_as_string")
        << (QJsonObject{{"id", "abc"}, {"name", "Test"}})
        << 0 << QString("Test");

    // null значение для name
    QTest::newRow("name_is_null")
        << (QJsonObject{{"id", 1}, {"name", QJsonValue::Null}})
        << 1 << QString("");
}

void TestEmployeeModel::testFromJsonEdgeCases()
{
    QFETCH(QJsonObject, inputJson);
    QFETCH(int, expectedId);
    QFETCH(QString, expectedName);

    model::Employee dept = model::Employee::fromJson(inputJson);

    QCOMPARE(dept.id, expectedId);
    QCOMPARE(dept.name, expectedName);
}

QTEST_MAIN(TestEmployeeModel)

#include "test_employee_model.moc"

