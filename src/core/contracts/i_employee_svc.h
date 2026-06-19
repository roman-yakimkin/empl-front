//
// Created by Roman Yakimkin on 11.06.2026.
//

#pragma once

#include <QObject>
#include "core/cmd/employee.h"
#include "core/models/employee.h"

namespace core {
    class IEmployeeSvc : public QObject {
        Q_OBJECT
    public:
        explicit IEmployeeSvc(QObject *parent = nullptr) : QObject(parent) {}
        ~IEmployeeSvc() override;

        virtual void getOne(int id) = 0;
        virtual void getAll() = 0;
        virtual void create(const cmd::employee::Create& cmd) = 0;
        virtual void update(const cmd::employee::Update& cmd) = 0;
        virtual void del(const cmd::employee::Delete& cmd) = 0;

    signals:
        void employeesLoaded(const QVector<model::Employee> &items);
        void employeeLoaded(const model::Employee& item);
        void employeeCreated(const model::Employee& item);
        void employeeUpdated(const model::Employee& item);
        void employeeDeleted(int id);

        void errorOccurred(const QString &code, const QString &message);
    };
} // core