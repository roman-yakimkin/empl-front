//
// Created by Roman Yakimkin on 10.06.2026.
//

#pragma once

#include <QString>
#include <QJsonObject>

#include "department.h"
#include "position.h"

namespace model {
    struct Employee {
        int id;
        QString name;
        Department department;
        Position position;
        QDateTime created_at;
        QDateTime updated_at;

        static Employee fromJson(const QJsonObject& json);
        QJsonObject toJson() const;
    };
} // model