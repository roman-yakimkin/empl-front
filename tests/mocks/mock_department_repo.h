//
// Created by Roman Yakimkin on 21.06.2026.
//

#pragma once

#include <QObject>

#include "core/contracts/i_department_repo.h"
#include "core/models/department.h"

class MockDepartmentRepo : public QObject, public core::IDepartmentRepo {
    Q_OBJECT
public:
    model::Department item_to_return;
    QVector<model::Department> items_to_return;
    QString error_to_return;
    bool bool_result_to_return = true;

    int last_id = 0;
    cmd::department::Create last_create_cmd;
    cmd::department::Update last_update_cmd;
    cmd::department::Delete last_delete_cmd;

    int get_one_call_count = 0;
    int get_all_call_count = 0;
    int create_call_count = 0;
    int update_call_count = 0;
    int del_call_count = 0;

    void getOne(int id, DepartmentCallback callback) override {
        last_id = id;
        get_one_call_count++;
        callback(item_to_return, error_to_return);
    }

    void getAll(DepartmentsCallback callback) override {
        get_all_call_count++;
        callback(items_to_return, error_to_return);
    }

    void create(const cmd::department::Create& cmd, DepartmentCallback callback) override {
        last_create_cmd = cmd;
        create_call_count++;
        callback(item_to_return, error_to_return);
    }

    void update(const cmd::department::Update& cmd, DepartmentCallback callback) override {
        last_update_cmd = cmd;
        update_call_count++;
        callback(item_to_return, error_to_return);
    }

    void del(const cmd::department::Delete& cmd, BoolCallback callback) override {
        last_delete_cmd = cmd;
        del_call_count++;
        callback(bool_result_to_return, error_to_return);
    }
};
