//
// Created by Roman Yakimkin on 11.06.2026.
//

#include <QPointer>

#include "employee.h"
#include <qloggingcategory.h>
#include "helpers/msg.h"
#include "infrastructure/logging.h"

namespace svc {
    void Employee::getOne(int id) {
        qCInfo(logApp) << "Loading employee with ID:" << id;

        QPointer self(this);

        m_repo->getOne(id, [self, id](const model::Employee& item, const QString& error){
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logApp) << "Failed to load employee: " << id << ":" << error;
                emit self->errorOccurred(msg::err::ERR_LOAD_EMPLOYEE, error);

                return;
            }

            qCInfo(logApp) << "Successfully loaded employee:" << item.name;
            emit self->employeeLoaded(item);
        });
    }

    void Employee::getAll() {
        qCInfo(logApp) << "Loading all employees...";

        QPointer self(this);

        m_repo->getAll([self](const QVector<model::Employee>& items, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logApp) << "Failed to load employees:" << error;
                emit self->errorOccurred(msg::err::ERR_LOAD_ALL_EMPLOYEES, error);
                return;
            }

            qCInfo(logApp) << "Successfully loaded" << items.size() << "employees";
            emit self->employeesLoaded(items);
        });
    }

    void Employee::create(const cmd::employee::Create &cmd) {
        qCInfo(logApp) << "Creating employee:" << cmd.name;

        QPointer self(this);

        m_repo->create(cmd, [self, cmd](const model::Employee& item, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logApp) << "Failed to create employee:" << error;
                emit self->errorOccurred(msg::err::ERR_CREATE_EMPLOYEE, error);

                return;
            }

            qCInfo(logApp) << "Successfully created employee:" << item.name << "with ID:" << item.id;
            emit self->employeeCreated(item);
        });
    }

    void Employee::update(const cmd::employee::Update &cmd) {
        qCInfo(logApp) << "Updating employee with ID:" << cmd.id;

        QPointer self(this);

        m_repo->update(cmd, [self, cmd](const model::Employee& item, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logApp) << "Failed to update employee" << cmd.id << ":" << error;
                emit self->errorOccurred(msg::err::ERR_UPDATE_EMPLOYEE, error);

                return;
            }

            qCInfo(logApp) << "Successfully updated employee:" << item.name;
            emit self->employeeUpdated(item);
        });
    }

    void Employee::del(const cmd::employee::Delete &cmd) {
        qCInfo(logApp) << "Deleting employee with ID:" << cmd.id;

        QPointer self(this);

        m_repo->del(cmd, [self, cmd](bool success, const QString& error) {
            if (!self) return;

            if (!success || !error.isEmpty()) {
                qCWarning(logApp) << "Failed to delete employee" << cmd.id << ":" << error;
                emit self->errorOccurred(msg::err::ERR_DELETE_EMPLOYEE, error);

                return;
            }

            qCInfo(logApp) << "Successfully deleted employee with ID:" << cmd.id;
            emit self->employeeDeleted(cmd.id);
        });

    }

} // svc