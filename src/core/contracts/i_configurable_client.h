//
// Created by Roman Yakimkin on 17.06.2026.
//

#pragma once

#include <QString>

namespace core {
    class IConfigurableClient {
    public:
        virtual ~IConfigurableClient() = default;

        virtual void setBaseUrl(const QString& baseUrl) = 0;
        virtual void setPort(int port) = 0;
        virtual void setTransferTimeout(int timeout) = 0;
    };
} // core