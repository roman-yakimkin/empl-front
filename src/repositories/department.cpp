//
// Created by Roman Yakimkin on 11.06.2026.
//

#include <QPointer>
#include <QJsonArray>

#include "department.h"
#include "core/models/department.h"
#include "infrastructure/logging.h"

namespace repo {
    void Department::getOne(int id, DepartmentCallback callback) {
        QPointer self(this);
        QString endpoint = QString("/departments/%1").arg(id);

        m_http_client->get(endpoint, [self, callback, id](const QJsonObject& json, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logNetwork) << "Failed to fetch department " << id << ":" << error;
                callback({}, error);

                return;
            }

            callback(model::Department::fromJson(json), "");
        });
    }

    void Department::getAll(DepartmentsCallback callback) {
        QPointer self(this);

        m_http_client->get("/departments", [self, callback](const QJsonObject& json, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logNetwork) << "Failed to fetch departments: " << error;
                callback({}, error);

                return;
            }

            if (!(json.contains("items") && json["items"].isArray())) {
                qCWarning(logNetwork);
                callback({}, "Неверный формат ответа сервера");

                return;
            }

            QVector<model::Department> departments;
            QJsonArray items = json["items"].toArray();
            departments.reserve(items.count());

            for (const QJsonValue& item : items) {
                if (item.isObject()) departments.append(model::Department::fromJson(item.toObject()));
            }

            qCInfo(logApp) << "Successfully fetched" << departments.size() << "departments";
            callback(departments, "");
        });
    }

    void Department::create(const cmd::department::Create &cmd, DepartmentCallback callback) {
        QPointer self(this);

        QJsonObject payload;
        payload["name"] = cmd.name;

        m_http_client->post("/departments", payload, [self, callback](const QJsonObject& json, const QString& error){
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logNetwork) << "Failed to create department:" << error;
                callback({}, error);

                return;
            }

            qCInfo(logApp) << "Successfully created department";
            callback(model::Department::fromJson(json), "");
        });
    }

    void Department::update(const cmd::department::Update &cmd, DepartmentCallback callback) {
        QPointer self(this);
        QString endpoint = QString("/departments/%1").arg(cmd.id);

        QJsonObject payload;
        payload["name"] = cmd.name;
        payload["id"] = cmd.id;

        m_http_client->put(endpoint, payload, [self, callback, cmd](const QJsonObject& json, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logNetwork) << "Failed to update department:" << cmd.id << ":" << error;
                callback({}, error);

                return;
            }

            qCInfo(logApp) << "Successfully updated department: " << cmd.id;
            callback(model::Department::fromJson(json), "");
        });
    }

    void Department::del(const cmd::department::Delete &cmd, BoolCallback callback) {
        QPointer self(this);
        QString endpoint = QString("/departments/%1").arg(cmd.id);

        m_http_client->del(endpoint, [self, callback, cmd](const QJsonObject&, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logNetwork) << "Failed to delete department: " << cmd.id << ":" << error;
                callback(false, error);

                return;
            }

            qCInfo(logApp) << "Successfully deleted department: " << cmd.id;
            callback(true, "");
        });
    }
} // repo