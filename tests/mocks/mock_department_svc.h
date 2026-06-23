// tests/mocks/mock_department_svc.h
// Мок-реализация IDepartmentSvc для UI-тестов

#pragma once

#include <QVector>
#include "core/contracts/i_department_svc.h"
#include "core/models/department.h"
#include "core/cmd/department.h"

class MockDepartmentSvc : public core::IDepartmentSvc {
    Q_OBJECT

public:
    explicit MockDepartmentSvc(QObject *parent = nullptr) : core::IDepartmentSvc(parent) {}

    void getOne(int /*id*/) override {}

    void getAll() override {
        get_all_called = true;
    }

    void create(const cmd::department::Create& /*cmd*/) override {
        create_called = true;
    }

    void update(const cmd::department::Update& /*cmd*/) override {
        update_called = true;
    }

    void del(const cmd::department::Delete& /*cmd*/) override {
        del_called = true;
    }

    bool get_all_called = false;
    bool create_called = false;
    bool update_called = false;
    bool del_called = false;

    void emitDepartmentsLoaded(const QVector<model::Department>& items) {
        emit departmentsLoaded(items);
    }

    void emitError(const QString& code, const QString& message) {
        emit errorOccurred(code, message);
    }

    void emitDepartmentCreated(const model::Department& item) {
        emit departmentCreated(item);
    }

    void emitDepartmentUpdated(const model::Department& item) {
        emit departmentUpdated(item);
    }

    void emitDepartmentDeleted(int id) {
        emit departmentDeleted(id);
    }
};