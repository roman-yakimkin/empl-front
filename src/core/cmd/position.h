//
// Created by Roman Yakimkin on 09.06.2026.
//

#pragma once

#include <QString>

namespace cmd::position {
    struct Create {
        QString name;
    };

    struct Update {
        int id;
        QString name;
    };

    struct Delete {
        int id;
    };
} // cmd