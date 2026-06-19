//
// Created by Roman Yakimkin on 09.06.2026.
//

#pragma once

#include <QString>

namespace cmd::employee {
    struct Create {
        QString name;
        int department_id;
        int position_id;
    };

    struct Update {
        int id;
        QString name;
        int department_id;
        int position_id;
    };

    struct Delete {
        int id;
    };
}