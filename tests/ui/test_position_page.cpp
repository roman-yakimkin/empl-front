//
// Created by Roman Yakimkin on 23.06.2026.
//

#include <QtTest>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTableView>
#include <QStackedWidget>
#include <QHeaderView>

#include "ui/positionpage.h"
#include "ui/models/table/position.h"
#include "../mocks/mock_position_svc.h"

class PositionPageTest : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Тесты навигации
    void test_addButton_switchesToInsertPage();
    void test_cancelButton_returnsToMainPage();

    // Тесты загрузки данных
    void test_reloadData_callsGetAll();
    void test_positionsLoaded_populatesModel();
    void test_positionsLoaded_selectsFirstRow();

    // Тесты создания/редактирования
    void test_saveButton_callsCreate_inInsertMode();
    void test_tableViewDblClick_entersEditMode();
    void test_saveButton_callsUpdate_inEditMode();

    // Тесты удаления
    void test_deleteButton_showsConfirmPage();
    void test_deleteConfirmButton_callsDelete();

private:
    MockPositionSvc* mock_svc = nullptr;
    views::PositionPage* page = nullptr;
};

void PositionPageTest::initTestCase() {
    // Инициализация для всех тестов (выполняется один раз в начале)
}

void PositionPageTest::cleanupTestCase() {
    // Очистка после всех тестов
}

void PositionPageTest::init() {
    // Выполняется перед каждым тестом
    mock_svc = new MockPositionSvc();
    page = new views::PositionPage(mock_svc);
    page->show();
    // Ждём, пока окно станет активным
    QVERIFY(QTest::qWaitForWindowActive(page));
}

void PositionPageTest::cleanup() {
    // Выполняется после каждого теста
    delete page;
    delete mock_svc;
    page = nullptr;
    mock_svc = nullptr;
}

// ТЕСТЫ
void PositionPageTest::test_addButton_switchesToInsertPage() {
    auto btn_add = page->findChild<QPushButton*>("btnAdd");
    QVERIFY2(btn_add != nullptr, "btnAdd not found");

    QTest::mouseClick(btn_add, Qt::LeftButton);

    auto stacked_widget = page->findChild<QStackedWidget*>("stackedWidget");
    auto pg_ins_upd = page->findChild<QWidget*>("pgInsUpd");
    auto table_view = page->findChild<QTableView*>("tableView");

    QCOMPARE(stacked_widget->currentWidget(), pg_ins_upd);
    QVERIFY(!table_view->isEnabled());
}

void PositionPageTest::test_cancelButton_returnsToMainPage() {
    // Сначала переходим в режим редактирования
    auto btn_add = page->findChild<QPushButton*>("btnAdd");
    QTest::mouseClick(btn_add, Qt::LeftButton);

    // Нажимаем Cancel
    auto btn_cancel = page->findChild<QPushButton*>("btnIUCancel");
    QTest::mouseClick(btn_cancel, Qt::LeftButton);

    auto stacked_widget = page->findChild<QStackedWidget*>("stackedWidget");
    auto pg_btn = page->findChild<QWidget*>("pgBtn");
    auto table_view = page->findChild<QTableView*>("tableView");

    QCOMPARE(stacked_widget->currentWidget(), pg_btn);
    QVERIFY(table_view->isEnabled());
}

void PositionPageTest::test_reloadData_callsGetAll() {
    mock_svc->get_all_called = false;
    page->reloadData();
    QVERIFY(mock_svc->get_all_called);
}

void PositionPageTest::test_positionsLoaded_populatesModel() {
    QVector<model::Position> test_items = {
        {1, QStringLiteral("Инженер")},
        {2, QStringLiteral("Бухгалтер")}
    };

    mock_svc->emitPositionsLoaded(test_items);

    auto table_view = page->findChild<QTableView*>("tableView");
    auto table_model = qobject_cast<views::models::table::Position*>(table_view->model());

    QCOMPARE(table_model->rowCount(QModelIndex()), 2);
    QCOMPARE(table_model->data(table_model->index(0, 1), Qt::DisplayRole).toString(), QStringLiteral("Инженер"));
    QCOMPARE(table_model->data(table_model->index(1, 1), Qt::DisplayRole).toString(), QStringLiteral("Бухгалтер"));
}

void PositionPageTest::test_positionsLoaded_selectsFirstRow() {
    QVector<model::Position> test_items = {
        {10, QStringLiteral("Водитель")}
    };

    mock_svc->emitPositionsLoaded(test_items);

    auto table_view = page->findChild<QTableView*>("tableView");
    auto selection_model = table_view->selectionModel();

    QVERIFY(selection_model->hasSelection());
    QVERIFY(selection_model->isSelected(table_view->model()->index(0, 0)));
}

void PositionPageTest::test_saveButton_callsCreate_inInsertMode() {
    // Переход в режим добавления
    auto btn_add = page->findChild<QPushButton*>("btnAdd");
    QTest::mouseClick(btn_add, Qt::LeftButton);

    // Ввод названия
    auto le_item_name = page->findChild<QLineEdit*>("leItemName");
    le_item_name->setText(QStringLiteral("Новая должность"));

    // Нажатие Save
    auto btn_save = page->findChild<QPushButton*>("btnSave");
    mock_svc->create_called = false;
    QTest::mouseClick(btn_save, Qt::LeftButton);

    QVERIFY(mock_svc->create_called);
}

void PositionPageTest::test_tableViewDblClick_entersEditMode() {
    // Загружаем данные
    QVector<model::Position> test_items = {{1, QStringLiteral("Инженер")}};
    mock_svc->emitPositionsLoaded(test_items);

    // Даем Qt обработать события и пересчитать layout таблицы
    qApp->processEvents();

    auto table_view = page->findChild<QTableView*>("tableView");
    QModelIndex index = table_view->model()->index(0, 0);

    // Убеждаемся, что таблица успела отрисоваться и визуальный прямоугольник валиден
    QVERIFY2(table_view->visualRect(index).isValid(),
             "visualRect is invalid! Table layout was not updated after setItems.");

    // Эмуляция двойного клика по центру ячейки
    emit table_view->doubleClicked(index);
    qApp->processEvents();

    // Проверка перехода в режим редактирования
    auto stacked_widget = page->findChild<QStackedWidget*>("stackedWidget");
    auto pg_ins_upd = page->findChild<QWidget*>("pgInsUpd");
    auto le_item_name = page->findChild<QLineEdit*>("leItemName");

    QCOMPARE(stacked_widget->currentWidget(), pg_ins_upd);
    QCOMPARE(le_item_name->text(), QStringLiteral("Инженер"));
}

void PositionPageTest::test_saveButton_callsUpdate_inEditMode() {
    // Загружаем данные и переходим в режим редактирования
    QVector<model::Position> test_items = {{1, QStringLiteral("Инженер")}};
    mock_svc->emitPositionsLoaded(test_items);

    auto table_view = page->findChild<QTableView*>("tableView");
    QModelIndex index = table_view->model()->index(0, 0);
    QPoint center = table_view->visualRect(index).center();
    QTest::mouseDClick(table_view->viewport(), Qt::LeftButton, Qt::NoModifier, center);

    // Меняем название и сохраняем
    auto le_item_name = page->findChild<QLineEdit*>("leItemName");
    le_item_name->setText(QStringLiteral("Инженер 1-й категории"));

    auto btn_save = page->findChild<QPushButton*>("btnSave");
    mock_svc->update_called = false;
    QTest::mouseClick(btn_save, Qt::LeftButton);

    QVERIFY(mock_svc->update_called);
}

void PositionPageTest::test_deleteButton_showsConfirmPage() {
    // Загружаем данные
    QVector<model::Position> test_items = {{5, QStringLiteral("Бухгалтер")}};
    mock_svc->emitPositionsLoaded(test_items);

    // Выбираем строку
    auto table_view = page->findChild<QTableView*>("tableView");
    table_view->setCurrentIndex(table_view->model()->index(0, 0));

    // Нажимаем Delete
    auto btn_del = page->findChild<QPushButton*>("btnDel");
    QTest::mouseClick(btn_del, Qt::LeftButton);

    // Проверка перехода на страницу подтверждения
    auto stacked_widget = page->findChild<QStackedWidget*>("stackedWidget");
    auto pg_del = page->findChild<QWidget*>("pgDel");
    auto lb_del_confirm = page->findChild<QLabel*>("lbDelConfirm");

    QCOMPARE(stacked_widget->currentWidget(), pg_del);
    QVERIFY(lb_del_confirm->text().contains(QStringLiteral("Бухгалтер")));
}

void PositionPageTest::test_deleteConfirmButton_callsDelete() {
    // Загружаем данные и выбираем строку
    QVector<model::Position> test_items = {{7, QStringLiteral("Менеджер")}};
    mock_svc->emitPositionsLoaded(test_items);

    auto table_view = page->findChild<QTableView*>("tableView");
    table_view->setCurrentIndex(table_view->model()->index(0, 0));

    // Переход на страницу удаления
    auto btn_del = page->findChild<QPushButton*>("btnDel");
    QTest::mouseClick(btn_del, Qt::LeftButton);

    // Подтверждение удаления
    auto btn_del_do = page->findChild<QPushButton*>("btnDelDo");
    mock_svc->del_called = false;
    QTest::mouseClick(btn_del_do, Qt::LeftButton);

    QVERIFY(mock_svc->del_called);
}

// Макрос для генерации main()
QTEST_MAIN(PositionPageTest)
#include "test_position_page.moc"

