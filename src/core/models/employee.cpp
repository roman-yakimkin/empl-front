//
// Created by Roman Yakimkin on 10.06.2026.
//

#include <QTimeZone>

#include "employee.h"

namespace model {
    Employee Employee::fromJson(const QJsonObject& json) {
        Employee emp;
        emp.id = json["id"].toInt();
        emp.name = json["name"].toString();

        // Если пришел полный ответ (с вложенными объектами)
        if (json.contains("department") && json["department"].isObject()) {
            emp.department = Department::fromJson(json["department"].toObject());
            emp.position = Position::fromJson(json["position"].toObject());
        }
        // Если пришел краткий ответ (только ID после создания/обновления)
        else {
            emp.department.id = json["department_id"].toInt();
            emp.position.id = json["position_id"].toInt();
        }

        qint64 createdTs = json["created_at"].toVariant().toLongLong();
        emp.created_at = QDateTime::fromSecsSinceEpoch(createdTs, QTimeZone::utc());

        qint64 updatedTs = json["updated_at"].toVariant().toLongLong();
        emp.updated_at = QDateTime::fromSecsSinceEpoch(updatedTs, QTimeZone::utc());

        return emp;
    }

    QJsonObject Employee::toJson() const {
        QJsonObject json;
        json["id"] = id;
        json["name"] = name;

        if (department.name.isEmpty())
            json["department_id"] = department.id;
        else
            json["department"] = department.toJson();

        if (position.name.isEmpty())
            json["position_id"] = position.id;
        else
            json["position"] = position.toJson();

        json["created_at"] = created_at.toSecsSinceEpoch();
        json["updated_at"] = updated_at.toSecsSinceEpoch();
        return json;
    }
}

