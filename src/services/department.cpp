//
// Created by Roman Yakimkin on 11.06.2026.
//

#include <QPointer>

#include "department.h"
#include <qloggingcategory.h>

#include "helpers/msg.h"
#include "infrastructure/logging.h"

namespace svc {
    void Department::getOne(int id) {
        qCInfo(logApp) << "Loading department with ID:" << id;

        QPointer self(this);

        m_repo->getOne(id, [self, id](const model::Department& item, const QString& error){
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logApp) << "Failed to load department: " << id << ":" << error;
                emit self->errorOccurred(msg::err::ERR_LOAD_DEPARTMENT, error);

                return;
            }

            qCInfo(logApp) << "Successfully loaded department:" << item.name;
            emit self->departmentLoaded(item);
        });
    }

    void Department::getAll() {
        qCInfo(logApp) << "Loading all departments...";

        QPointer self(this);

        m_repo->getAll([self](const QVector<model::Department>& items, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logApp) << "Failed to load departments:" << error;
                emit self->errorOccurred(msg::err::ERR_LOAD_ALL_DEPARTMENTS, error);
                return;
            }

            qCInfo(logApp) << "Successfully loaded" << items.size() << "departments";
            emit self->departmentsLoaded(items);
        });
    }

    void Department::create(const cmd::department::Create &cmd) {
        qCInfo(logApp) << "Creating department:" << cmd.name;

        QPointer self(this);

        m_repo->create(cmd, [self, cmd](const model::Department& item, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logApp) << "Failed to create department:" << error;
                emit self->errorOccurred(msg::err::ERR_CREATE_DEPARTMENT, error);

                return;
            }

            qCInfo(logApp) << "Successfully created department:" << item.name << "with ID:" << item.id;
            emit self->departmentCreated(item);
        });
    }

    void Department::update(const cmd::department::Update &cmd) {
        qCInfo(logApp) << "Updating department with ID:" << cmd.id;

        QPointer self(this);

        m_repo->update(cmd, [self, cmd](const model::Department& item, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logApp) << "Failed to update department" << cmd.id << ":" << error;
                emit self->errorOccurred(msg::err::ERR_UPDATE_DEPARTMENT, error);

                return;
            }

            qCInfo(logApp) << "Successfully updated department:" << item.name;
            emit self->departmentUpdated(item);
        });
    }

    void Department::del(const cmd::department::Delete &cmd) {
        qCInfo(logApp) << "Deleting department with ID:" << cmd.id;

        QPointer self(this);

        m_repo->del(cmd, [self, cmd](bool success, const QString& error) {
            if (!self) return;

            if (!success || !error.isEmpty()) {
                qCWarning(logApp) << "Failed to delete department" << cmd.id << ":" << error;
                emit self->errorOccurred(msg::err::ERR_DELETE_DEPARTMENT, error);

                return;
            }

            qCInfo(logApp) << "Successfully deleted department with ID:" << cmd.id;
            emit self->departmentDeleted(cmd.id);
        });

    }
} // svc