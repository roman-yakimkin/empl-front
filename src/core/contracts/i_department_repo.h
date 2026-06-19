//
// Created by Roman Yakimkin on 11.06.2026.
//

#pragma once

#include <QString>
#include <QVector>

#include "core/cmd/department.h"
#include "core/models/department.h"

namespace core {
    class IDepartmentRepo {
    public:
        virtual ~IDepartmentRepo() = default;

        using DepartmentsCallback = std::function<void(const QVector<model::Department>&, const QString&)>;
        using DepartmentCallback = std::function<void(const model::Department&, const QString&)>;
        using BoolCallback = std::function<void(bool, const QString&)>;

        virtual void getOne(int id, DepartmentCallback callback) = 0;
        virtual void getAll(DepartmentsCallback callback) = 0;
        virtual void create(const cmd::department::Create& cmd, DepartmentCallback callback) = 0;
        virtual void update(const cmd::department::Update& cmd, DepartmentCallback callback) = 0;
        virtual void del(const cmd::department::Delete& cmd, BoolCallback callback) = 0;
    };
} // core