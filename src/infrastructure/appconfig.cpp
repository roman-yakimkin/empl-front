//
// Created by Roman Yakimkin on 11.06.2026.
//

#include "appconfig.h"

#include <qcoreapplication.h>

namespace infra {
    AppConfig::AppConfig() : m_settings(QSettings::IniFormat, QSettings::UserScope,
                                      QCoreApplication::organizationName(), QCoreApplication::applicationName()) {
        if (!m_settings.contains(API_BASE_URL))
            m_settings.setValue(API_BASE_URL, DEFAULT_API_URL);
        if (!m_settings.contains(API_PORT))
            m_settings.setValue(API_PORT, DEFAULT_API_PORT);
        if (!m_settings.contains(API_TIMEOUT_SEC))
            m_settings.setValue(API_TIMEOUT_SEC, DEFAULT_API_TIMEOUT_SEC);

        m_settings.sync();
    }

    QString AppConfig::apiBaseUrl() const {
        return m_settings.value(API_BASE_URL, DEFAULT_API_URL).toString();
    }

    void AppConfig::setApiBaseUrl(const QString &url) {
        m_settings.setValue(API_BASE_URL, url);
    }

    int AppConfig::apiPort() const {
        return m_settings.value(API_PORT, DEFAULT_API_PORT).toInt();
    }

    void AppConfig::setApiPort(const int port) {
        m_settings.setValue(API_PORT, port);
    }

    int AppConfig::apiTimeout() const {
        return m_settings.value(API_TIMEOUT_SEC, DEFAULT_API_TIMEOUT_SEC).toInt();
    }

    void AppConfig::setApiTimeout(const int timeout) {
        m_settings.setValue(API_TIMEOUT_SEC, timeout);
    }

    void AppConfig::resetToDefaults() {
        m_settings.setValue(API_BASE_URL, DEFAULT_API_URL);
        m_settings.setValue(API_PORT, DEFAULT_API_PORT);
        m_settings.setValue(API_TIMEOUT_SEC, DEFAULT_API_TIMEOUT_SEC);
        m_settings.sync();
    }
} // infra