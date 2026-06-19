//
// Created by Roman Yakimkin on 11.06.2026.
//

#pragma once

#include "core/cmd/employee.h"
#include "core/models/employee.h"

namespace core {
    class IEmployeeRepo {
    public:
        virtual ~IEmployeeRepo() = default;

        using EmployeesCallback = std::function<void(const QVector<model::Employee>&, const QString&)>;
        using EmployeeCallback = std::function<void(const model::Employee&, const QString&)>;
        using BoolCallback = std::function<void(bool, const QString&)>;

        virtual void getOne(int id, EmployeeCallback callback) = 0;
        virtual void getAll(EmployeesCallback callback) = 0;
        virtual void create(const cmd::employee::Create& cmd, EmployeeCallback callback) = 0;
        virtual void update(const cmd::employee::Update& cmd, EmployeeCallback callback) = 0;
        virtual void del(const cmd::employee::Delete& cmd, BoolCallback callback) = 0;
    };
} // core