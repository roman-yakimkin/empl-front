//
// Created by Roman Yakimkin on 11.06.2026.
//

#pragma once

#include <QObject>

#include "core/cmd/department.h"
#include "core/models/department.h"

namespace core {
    class IDepartmentSvc : public QObject {
        Q_OBJECT
    public:
        explicit IDepartmentSvc(QObject *parent = nullptr) : QObject(parent) {}
        ~IDepartmentSvc() override;

        virtual void getOne(int id) = 0;
        virtual void getAll() = 0;
        virtual void create(const cmd::department::Create& cmd) = 0;
        virtual void update(const cmd::department::Update& cmd) = 0;
        virtual void del(const cmd::department::Delete& cmd) = 0;

    signals:
        void departmentsLoaded(const QVector<model::Department> &items);
        void departmentLoaded(const model::Department& item);
        void departmentCreated(const model::Department& item);
        void departmentUpdated(const model::Department& item);
        void departmentDeleted(int id);

        void errorOccurred(const QString &code, const QString &message);
    };
} // core