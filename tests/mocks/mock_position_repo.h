//
// Created by Roman Yakimkin on 22.06.2026.
//

#pragma once

#include <QObject>

#include "core/contracts/i_position_repo.h"
#include "core/models/position.h"

class MockPositionRepo : public QObject, public core::IPositionRepo {
    Q_OBJECT
public:
    model::Position item_to_return;
    QVector<model::Position> items_to_return;
    QString error_to_return;
    bool bool_result_to_return = true;

    int last_id = 0;
    cmd::position::Create last_create_cmd;
    cmd::position::Update last_update_cmd;
    cmd::position::Delete last_delete_cmd;

    int get_one_call_count = 0;
    int get_all_call_count = 0;
    int create_call_count = 0;
    int update_call_count = 0;
    int del_call_count = 0;

    void getOne(int id, PositionCallback callback) override {
        last_id = id;
        get_one_call_count++;
        callback(item_to_return, error_to_return);
    }

    void getAll(PositionsCallback callback) override {
        get_all_call_count++;
        callback(items_to_return, error_to_return);
    }

    void create(const cmd::position::Create& cmd, PositionCallback callback) override {
        last_create_cmd = cmd;
        create_call_count++;
        callback(item_to_return, error_to_return);
    }

    void update(const cmd::position::Update& cmd, PositionCallback callback) override {
        last_update_cmd = cmd;
        update_call_count++;
        callback(item_to_return, error_to_return);
    }

    void del(const cmd::position::Delete& cmd, BoolCallback callback) override {
        last_delete_cmd = cmd;
        del_call_count++;
        callback(bool_result_to_return, error_to_return);
    }
};
