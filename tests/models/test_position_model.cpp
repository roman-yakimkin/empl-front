//
// Created by Roman Yakimkin on 20.06.2026.
//

#include <QtTest>
#include "core/models/position.h"

class TestPositionModel : public QObject {
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

void TestPositionModel::testDefaultConstructor() {
    model::Position item;
    QCOMPARE(item.id, 0);
    QVERIFY(item.name.isEmpty());
}

void TestPositionModel::testParametrizedConstructor() {
    model::Position item(12, "Инженер");
    QCOMPARE(item.id, 12);
    QCOMPARE(item.name, QString("Инженер"));
}

void TestPositionModel::testFromJson() {
    model::Position item(42, "Инженер");
    QJsonObject json = item.toJson();

    QCOMPARE(json["id"].toInt(), 42);
    QCOMPARE(json["name"].toString(), QString("Инженер"));
    QCOMPARE(json.size(), 2);
}

void TestPositionModel::testToJson() {
    QJsonObject json;
    json["id"] = 100;
    json["name"] = "Инженер";

    model::Position item = model::Position::fromJson(json);
    QCOMPARE(item.id, 100);
    QCOMPARE(item.name, QString("Инженер"));
}

void TestPositionModel::testRoundTrip() {
    model::Position original(101, "Инженер");

    QJsonObject json = original.toJson();
    model::Position restored = model::Position::fromJson(json);

    QCOMPARE(restored.id, 101);
    QCOMPARE(restored.name, QString("Инженер"));
}

void TestPositionModel::testFromJsonEdgeCases_data()
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

void TestPositionModel::testFromJsonEdgeCases()
{
    QFETCH(QJsonObject, inputJson);
    QFETCH(int, expectedId);
    QFETCH(QString, expectedName);

    model::Position dept = model::Position::fromJson(inputJson);

    QCOMPARE(dept.id, expectedId);
    QCOMPARE(dept.name, expectedName);
}

QTEST_MAIN(TestPositionModel)

#include "test_position_model.moc"

