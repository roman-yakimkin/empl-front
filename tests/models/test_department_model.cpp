//
// Created by Roman Yakimkin on 20.06.2026.
//

#include <QtTest>
#include "core/models/department.h"

class TestDepartmentModel : public QObject {
    Q_OBJECT

private slots:
    void testDefaultConstructor();
    void testParametrizedConstructor();

    void testFromJson();
    void testToJson();
    void testRoundTrip();

    void testFromJsonEdgeCases_data();
    void testFromJsonEdgeCases();
};

void TestDepartmentModel::testDefaultConstructor() {
    model::Department item;
    QCOMPARE(item.id, 0);
    QVERIFY(item.name.isEmpty());
}

void TestDepartmentModel::testParametrizedConstructor() {
    model::Department item(12, "Отдел АСУ");
    QCOMPARE(item.id, 12);
    QCOMPARE(item.name, QString("Отдел АСУ"));
}

void TestDepartmentModel::testFromJson() {
    model::Department item(42, "Отдел АСУ");
    QJsonObject json = item.toJson();

    QCOMPARE(json["id"].toInt(), 42);
    QCOMPARE(json["name"].toString(), QString("Отдел АСУ"));
    QCOMPARE(json.size(), 2);
}

void TestDepartmentModel::testToJson() {
    QJsonObject json;
    json["id"] = 100;
    json["name"] = "Отдел АСУ";

    model::Department item = model::Department::fromJson(json);
    QCOMPARE(item.id, 100);
    QCOMPARE(item.name, QString("Отдел АСУ"));
}

void TestDepartmentModel::testRoundTrip() {
    model::Department original(101, "Плановый отдел");

    QJsonObject json = original.toJson();
    model::Department restored = model::Department::fromJson(json);

    QCOMPARE(restored.id, 101);
    QCOMPARE(restored.name, QString("Плановый отдел"));
}

void TestDepartmentModel::testFromJsonEdgeCases_data()
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

void TestDepartmentModel::testFromJsonEdgeCases()
{
    QFETCH(QJsonObject, inputJson);
    QFETCH(int, expectedId);
    QFETCH(QString, expectedName);

    model::Department dept = model::Department::fromJson(inputJson);

    QCOMPARE(dept.id, expectedId);
    QCOMPARE(dept.name, expectedName);
}

QTEST_MAIN(TestDepartmentModel)

#include "test_department_model.moc"
