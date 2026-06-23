//
// Created by Roman Yakimkin on 23.06.2026.
//
// tests/mocks/mock_employee_svc.h
#pragma once

#include <QObject>
#include <QVector>
#include "core/contracts/i_employee_svc.h"
#include "core/models/employee.h"
#include "core/cmd/employee.h"

class MockEmployeeSvc : public core::IEmployeeSvc {
    Q_OBJECT

public:
    explicit MockEmployeeSvc(QObject *parent = nullptr) : core::IEmployeeSvc(parent) {}

    void getOne(int /*id*/) override {}
    void getAll() override { get_all_called = true; }
    void create(const cmd::employee::Create& cmd) override {
        create_called = true;
        last_create_cmd = cmd;
    }
    void update(const cmd::employee::Update& cmd) override {
        update_called = true;
        last_update_cmd = cmd;
    }
    void del(const cmd::employee::Delete& /*cmd*/) override { del_called = true; }

    bool get_all_called = false;
    bool create_called = false;
    bool update_called = false;
    bool del_called = false;

    cmd::employee::Create last_create_cmd;
    cmd::employee::Update last_update_cmd;

    void emitEmployeesLoaded(const QVector<model::Employee>& items) { emit employeesLoaded(items); }
    void emitEmployeeCreated(const model::Employee& item) { emit employeeCreated(item); }
    void emitEmployeeUpdated(const model::Employee& item) { emit employeeUpdated(item); }
    void emitEmployeeDeleted(int id) { emit employeeDeleted(id); }
    void emitError(const QString& code, const QString& message) { emit errorOccurred(code, message); }
};

