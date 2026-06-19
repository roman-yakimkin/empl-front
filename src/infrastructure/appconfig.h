//
// Created by Roman Yakimkin on 11.06.2026.
//

#pragma once

#include <QSettings>
#include <QString>

namespace infra {
    class AppConfig {
        QSettings m_settings;

        static constexpr const char* DEFAULT_API_URL = "http://localhost";
        static constexpr int DEFAULT_API_PORT = 8080;
        static constexpr int DEFAULT_API_TIMEOUT_SEC = 30;

        static constexpr const char* API_BASE_URL      = "Api/BaseUrl";
        static constexpr const char* API_PORT          = "Api/Port";
        static constexpr const char* API_TIMEOUT_SEC   = "Api/TimeoutSec";
    public:
        AppConfig();

        // Геттеры и сеттеры
        QString apiBaseUrl() const;
        void setApiBaseUrl(const QString& url);

        int apiPort() const;
        void setApiPort(int port);

        int apiTimeout() const;
        void setApiTimeout(int timeout);

        // Сброс к значениям по умолчанию
        void resetToDefaults();
    };
} // infra