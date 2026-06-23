//
// Created by Roman Yakimkin on 23.06.2026.
//
#pragma once

#include <QVector>
#include "core/contracts/i_position_svc.h"
#include "core/models/position.h"
#include "core/cmd/position.h"

class MockPositionSvc : public core::IPositionSvc {
    Q_OBJECT

public:
    explicit MockPositionSvc(QObject *parent = nullptr) : core::IPositionSvc(parent) {}

    void getOne(int /*id*/) override {}

    void getAll() override {
        get_all_called = true;
    }

    void create(const cmd::position::Create& /*cmd*/) override {
        create_called = true;
    }

    void update(const cmd::position::Update& /*cmd*/) override {
        update_called = true;
    }

    void del(const cmd::position::Delete& /*cmd*/) override {
        del_called = true;
    }

    bool get_all_called = false;
    bool create_called = false;
    bool update_called = false;
    bool del_called = false;

    void emitPositionsLoaded(const QVector<model::Position>& items) {
        emit positionsLoaded(items);
    }

    void emitError(const QString& code, const QString& message) {
        emit errorOccurred(code, message);
    }

    void emitPositionCreated(const model::Position& item) {
        emit positionCreated(item);
    }

    void emitPositionUpdated(const model::Position& item) {
        emit positionUpdated(item);
    }

    void emitPositionDeleted(int id) {
        emit positionDeleted(id);
    }
};