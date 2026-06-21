//
// Created by Roman Yakimkin on 12.06.2026.
//

#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>

#include "core/contracts/i_configurable_client.h"
#include "core/contracts/i_http_client.h"

namespace infra {
    class HttpClient : public QObject, public core::IConfigurableClient, public core::IHttpClient {
        Q_OBJECT
    private:
        QNetworkAccessManager* m_manager = nullptr;
        QString m_base_url;
        int m_port = 8080;
        int m_timeout = 30000;
        QHash<QString, QString> m_default_handlers;

    public:
        explicit HttpClient(QString baseUrl, int port, int timeOutSec, QObject* parent = nullptr);
        ~HttpClient() override = default;

        // Изменение базового URL (например, после смены настроек)
        void setBaseUrl(const QString& baseUrl) override;
        QString baseUrl() const;

        void setPort(int port) override;

        // Установка тайм-аута (в миллисекундах)
        void setTransferTimeout(int msecs) override;

        // Установка заголовка по умолчанию
        void setDefaultHeader(const QString& name, const QString& value);
        void clearDefaultHeaders();

        // HTTP методы
        void get(const QString& endpoint, const JsonCallback& callback) override;
        void post(const QString& endpoint, const QJsonObject& payload, const JsonCallback& callback) override;
        void put(const QString& endpoint, const QJsonObject& payload, const JsonCallback& callback) override;
        void del(const QString& endpoint, const JsonCallback& callback) override;

    signals:
        void requestStarted();
        void requestFinished();
    private:
        QNetworkRequest createRequest(const QString& endpoint) const;
        void sendRequest(QNetworkReply* reply, const JsonCallback& callback);
        static void handleReply(QNetworkReply* reply, const JsonCallback& callback);
    };
} // infra