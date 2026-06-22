//
// Created by Roman Yakimkin on 22.06.2026.
//

#pragma once
#include <QObject>

#include "core/contracts/i_employee_repo.h"

class MockEmployeeRepo : public QObject, public core::IEmployeeRepo {
    Q_OBJECT
public:
    // Данные для возврата
    model::Employee emp_to_return;
    QVector<model::Employee> emps_to_return;
    QString error_to_return;
    bool bool_result_to_return = true;

    // Для проверки вызовов
    int last_id = 0;
    cmd::employee::Create last_create_cmd;
    cmd::employee::Update last_update_cmd;
    cmd::employee::Delete last_delete_cmd;

    int get_one_call_count = 0;
    int get_all_call_count = 0;
    int create_call_count = 0;
    int update_call_count = 0;
    int del_call_count = 0;

    void getOne(int id, EmployeeCallback callback) override {
        last_id = id;
        get_one_call_count++;
        callback(emp_to_return, error_to_return);
    }

    void getAll(EmployeesCallback callback) override {
        get_all_call_count++;
        callback(emps_to_return, error_to_return);
    }

    void create(const cmd::employee::Create& cmd, EmployeeCallback callback) override {
        last_create_cmd = cmd;
        create_call_count++;
        callback(emp_to_return, error_to_return);
    }

    void update(const cmd::employee::Update& cmd, EmployeeCallback callback) override {
        last_update_cmd = cmd;
        update_call_count++;
        callback(emp_to_return, error_to_return);
    }

    void del(const cmd::employee::Delete& cmd, BoolCallback callback) override {
        last_delete_cmd = cmd;
        del_call_count++;
        callback(bool_result_to_return, error_to_return);
    }
};
