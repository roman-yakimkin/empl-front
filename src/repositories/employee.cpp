//
// Created by Roman Yakimkin on 11.06.2026.
//

#include <QPointer>
#include <QJsonArray>

#include "employee.h"
#include "core/models/employee.h"
#include "infrastructure/logging.h"

namespace repo {
    void Employee::getOne(int id, EmployeeCallback callback) {
        QPointer self(this);
        QString endpoint = QString("/employees/%1").arg(id);

        m_http_client->get(endpoint, [self, callback, id](const QJsonObject& json, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logNetwork) << "Failed to fetch employee " << id << ":" << error;
                callback({}, error);

                return;
            }

            callback(model::Employee::fromJson(json), "");
        });
    }

    void Employee::getAll(EmployeesCallback callback) {
        QPointer self(this);

        m_http_client->get("/employees", [self, callback](const QJsonObject& json, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logNetwork) << "Failed to fetch employees: " << error;
                callback({}, error);

                return;
            }

            if (!(json.contains("items") && json["items"].isArray())) {
                qCWarning(logNetwork);
                callback({}, "Неверный формат ответа сервера");

                return;
            }

            QVector<model::Employee> employees;
            QJsonArray items = json["items"].toArray();
            employees.reserve(items.count());

            for (const QJsonValue& item : items) {
                if (item.isObject()) employees.append(model::Employee::fromJson(item.toObject()));
            }

            qCInfo(logApp) << "Successfully fetched" << employees.size() << "employees";
            callback(employees, "");
        });
    }

    void Employee::create(const cmd::employee::Create &cmd, EmployeeCallback callback) {
        QPointer self(this);

        QJsonObject payload;
        payload["name"] = cmd.name;
        payload["department_id"] = cmd.department_id;
        payload["position_id"] = cmd.position_id;

        m_http_client->post("/employees", payload, [self, callback](const QJsonObject& json, const QString& error){
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logNetwork) << "Failed to create employee:" << error;
                callback({}, error);

                return;
            }

            qCInfo(logApp) << "Successfully created employee";
            callback(model::Employee::fromJson(json), "");
        });
    }

    void Employee::update(const cmd::employee::Update &cmd, EmployeeCallback callback) {
        QPointer self(this);
        QString endpoint = QString("/employees/%1").arg(cmd.id);

        QJsonObject payload;
        payload["name"] = cmd.name;
        payload["id"] = cmd.id;
        payload["department_id"] = cmd.department_id;
        payload["position_id"] = cmd.position_id;

        m_http_client->put(endpoint, payload, [self, callback, cmd](const QJsonObject& json, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logNetwork) << "Failed to update employee:" << cmd.id << ":" << error;
                callback({}, error);

                return;
            }

            qCInfo(logApp) << "Successfully updated employee: " << cmd.id;
            callback(model::Employee::fromJson(json), "");
        });
    }

    void Employee::del(const cmd::employee::Delete &cmd, BoolCallback callback) {
        QPointer self(this);
        QString endpoint = QString("/employees/%1").arg(cmd.id);

        m_http_client->del(endpoint, [self, callback, cmd](const QJsonObject&, const QString& error) {
            if (!self) return;

            if (!error.isEmpty()) {
                qCWarning(logNetwork) << "Failed to delete employee: " << cmd.id << ":" << error;
                callback(false, error);

                return;
            }

            qCInfo(logApp) << "Successfully deleted employee: " << cmd.id;
            callback(true, "");
        });
    }

} // repo