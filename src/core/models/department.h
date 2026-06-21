//
// Created by Roman Yakimkin on 09.06.2026.
//

#pragma once

#include <QJsonObject>
#include <QString>

namespace model {
    struct Department {
        int id = 0;
        QString name;

        Department() = default;
        Department(const int id, const QString& name): id(id), name(name) {}

        static Department fromJson(const QJsonObject& json) {
            return {json["id"].toInt(), json["name"].toString()};
        }

        QJsonObject toJson() const {
            QJsonObject json;

            json["id"] = id;
            json["name"] = name;

            return json;
        }
    };
} // model