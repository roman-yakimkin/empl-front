//
// Created by Roman Yakimkin on 11.06.2026.
//

#pragma once

#include "core/models/position.h"
#include "core/cmd/position.h"

namespace core {
    class IPositionRepo {
    public:
        virtual ~IPositionRepo() = default;

        using PositionsCallback = std::function<void(const QVector<model::Position>&, const QString&)>;
        using PositionCallback = std::function<void(const model::Position&, const QString&)>;
        using BoolCallback = std::function<void(bool, const QString&)>;

        virtual void getOne(int id, PositionCallback callback) = 0;
        virtual void getAll(PositionsCallback callback) = 0;
        virtual void create(const cmd::position::Create& cmd, PositionCallback callback) = 0;
        virtual void update(const cmd::position::Update& cmd, PositionCallback callback) = 0;
        virtual void del(const cmd::position::Delete& cmd, BoolCallback callback) = 0;

    };
} // core