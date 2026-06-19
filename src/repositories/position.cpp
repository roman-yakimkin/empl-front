//
// Created by Roman Yakimkin on 11.06.2026.
//

#include <QPointer>
#include <QJsonArray>

#include "position.h"
#include "core/models/position.h"
#include "infrastructure/logging.h"

namespace repo {
    void Position::getOne(int id, PositionCallback callback) {
        QPointer self(this);
        QString endpoint = QString("/positions/%1").arg(id);

        m_http_client->get(endpoint, [self, callback, id](const QJsonObject& json, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logNetwork) << "Failed to fetch Position " << id << ":" << error;
                callback({}, error);

                return;
            }

            callback(model::Position::fromJson(json), "");
        });
    }

    void Position::getAll(PositionsCallback callback) {
        QPointer self(this);

        m_http_client->get("/positions", [self, callback](const QJsonObject& json, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logNetwork) << "Failed to fetch Positions: " << error;
                callback({}, error);

                return;
            }

            if (!(json.contains("items") && json["items"].isArray())) {
                qCWarning(logNetwork);
                callback({}, "Неверный формат ответа сервера");

                return;
            }

            QVector<model::Position> positions;
            QJsonArray items = json["items"].toArray();
            positions.reserve(items.count());

            for (const QJsonValue& item : items) {
                if (item.isObject()) positions.append(model::Position::fromJson(item.toObject()));
            }

            qCInfo(logApp) << "Successfully fetched" << positions.size() << "position(s)";
            callback(positions, "");
        });
    }

    void Position::create(const cmd::position::Create &cmd, PositionCallback callback) {
        QPointer self(this);

        QJsonObject payload;
        payload["name"] = cmd.name;

        m_http_client->post("/positions", payload, [self, callback](const QJsonObject& json, const QString& error){
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logNetwork) << "Failed to create position:" << error;
                callback({}, error);

                return;
            }

            qCInfo(logApp) << "Successfully created Position";
            callback(model::Position::fromJson(json), "");
        });
    }

    void Position::update(const cmd::position::Update &cmd, PositionCallback callback) {
        QPointer self(this);
        QString endpoint = QString("/positions/%1").arg(cmd.id);

        QJsonObject payload;
        payload["name"] = cmd.name;
        payload["id"] = cmd.id;

        m_http_client->put(endpoint, payload, [self, callback, cmd](const QJsonObject& json, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logNetwork) << "Failed to update position:" << cmd.id << ":" << error;
                callback({}, error);

                return;
            }

            qCInfo(logApp) << "Successfully updated position: " << cmd.id;
            callback(model::Position::fromJson(json), "");
        });
    }

    void Position::del(const cmd::position::Delete &cmd, BoolCallback callback) {
        QPointer self(this);
        QString endpoint = QString("/positions/%1").arg(cmd.id);

        m_http_client->del(endpoint, [self, callback, cmd](const QJsonObject&, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logNetwork) << "Failed to delete position: " << cmd.id << ":" << error;
                callback(false, error);

                return;
            }

            qCInfo(logApp) << "Successfully deleted position: " << cmd.id;
            callback(true, "");
        });
    }
} // repo