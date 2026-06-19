//
// Created by Roman Yakimkin on 11.06.2026.
//

#include <QPointer>

#include "position.h"
#include <qloggingcategory.h>

#include "helpers/msg.h"
#include "infrastructure/logging.h"

namespace svc {
    void Position::getOne(int id) {
        qCInfo(logApp) << "Loading Position with ID:" << id;

        QPointer self(this);

        m_repo->getOne(id, [self, id](const model::Position& item, const QString& error){
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logApp) << "Failed to load position: " << id << ":" << error;
                emit self->errorOccurred(msg::err::ERR_LOAD_POSITION, error);

                return;
            }

            qCInfo(logApp) << "Successfully loaded position:" << item.name;
            emit self->positionLoaded(item);
        });
    }

    void Position::getAll() {
        qCInfo(logApp) << "Loading all positions...";

        QPointer self(this);

        m_repo->getAll([self](const QVector<model::Position>& items, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logApp) << "Failed to load positions:" << error;
                emit self->errorOccurred(msg::err::ERR_LOAD_ALL_POSITIONS, error);
                return;
            }

            qCInfo(logApp) << "Successfully loaded" << items.size() << "positions";
            emit self->positionsLoaded(items);
        });
    }

    void Position::create(const cmd::position::Create &cmd) {
        qCInfo(logApp) << "Creating position:" << cmd.name;

        QPointer self(this);

        m_repo->create(cmd, [self, cmd](const model::Position& item, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logApp) << "Failed to create position:" << error;
                emit self->errorOccurred(msg::err::ERR_CREATE_POSITION, error);

                return;
            }

            qCInfo(logApp) << "Successfully created position:" << item.name << "with ID:" << item.id;
            emit self->positionCreated(item);
        });
    }

    void Position::update(const cmd::position::Update &cmd) {
        qCInfo(logApp) << "Updating Position with ID:" << cmd.id;

        QPointer self(this);

        m_repo->update(cmd, [self, cmd](const model::Position& item, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logApp) << "Failed to update position" << cmd.id << ":" << error;
                emit self->errorOccurred(msg::err::ERR_UPDATE_POSITION, error);

                return;
            }

            qCInfo(logApp) << "Successfully updated position:" << item.name;
            emit self->positionUpdated(item);
        });
    }

    void Position::del(const cmd::position::Delete &cmd) {
        qCInfo(logApp) << "Deleting position with ID:" << cmd.id;

        QPointer self(this);

        m_repo->del(cmd, [self, cmd](bool success, const QString& error) {
            if (!self) return;

            if (!success || !error.isEmpty()) {
                qCWarning(logApp) << "Failed to delete position" << cmd.id << ":" << error;
                emit self->errorOccurred(msg::err::ERR_DELETE_POSITION, error);

                return;
            }

            qCInfo(logApp) << "Successfully deleted position with ID:" << cmd.id;
            emit self->positionDeleted(cmd.id);
        });

    }
} // svc