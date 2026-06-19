//
// Created by Roman Yakimkin on 12.06.2026.
//

#include <qloggingcategory.h>
#include <utility>
#include <QUrl>
#include <QNetworkReply>
#include <QJsonDocument>

#include "httpclient.h"
#include "logging.h"

namespace infra {
    HttpClient::HttpClient(QString base_url, int port, int timeout_sec, QObject *parent)
        : QObject(parent),
          m_manager(new QNetworkAccessManager(this)),
          m_base_url(std::move(base_url)),
          m_port(port),
          m_timeout(timeout_sec) {
        m_manager->setTransferTimeout(m_timeout);

        qCInfo(logNetwork) << "HttpClient initialized with base URL:" << m_base_url;
    }

    void HttpClient::setBaseUrl(const QString& base_url) {
        m_base_url = base_url;
    }

    QString HttpClient::baseUrl() const {
        return m_base_url;
    }

    void HttpClient::setPort(int port) {
        m_port = port;
    }

    void HttpClient::setTransferTimeout(int msecs) {
        m_timeout = msecs;
        m_manager->setTransferTimeout(m_timeout);

        qCDebug(logNetwork) << "Transfer timeout set to" << msecs << "ms";
    }

    void HttpClient::setDefaultHeader(const QString& name, const QString& value) {
        m_default_handlers[name] = value;
    }

    void HttpClient::clearDefaultHeaders() {
        m_default_handlers.clear();
    }

    QNetworkRequest HttpClient::createRequest(const QString &endpoint) const {
        const QUrl url(m_base_url + ":" + QString::number(m_port) + endpoint);
        QNetworkRequest req(url);

        for (auto it = m_default_handlers.begin(); it != m_default_handlers.end(); ++it) {
            req.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
        }

        return req;
    }

    void HttpClient::get(const QString &endpoint, const JsonCallback& callback) {
        const QNetworkRequest req = createRequest(endpoint);

        qCDebug(logNetwork) << "GET" << req.url().toString();

        emit requestStarted();
        QNetworkReply* reply = m_manager->get(req);
        sendRequest(reply, callback);
    }

    void HttpClient::post(const QString &endpoint, const QJsonObject &payload, const JsonCallback& callback) {
        QNetworkRequest req = createRequest(endpoint);
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        const QJsonDocument doc(payload);
        const QByteArray jsonData = doc.toJson(QJsonDocument::Compact);

        qCDebug(logNetwork) << "POST" << req.url().toString()
                    << "payload:" << jsonData;

        emit requestStarted();
        QNetworkReply* reply = m_manager->post(req, jsonData);
        sendRequest(reply, callback);
    }

    void HttpClient::put(const QString &endpoint, const QJsonObject &payload, const JsonCallback& callback) {
        QNetworkRequest req = createRequest(endpoint);
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        const QJsonDocument doc(payload);
        QByteArray jsonData = doc.toJson(QJsonDocument::Compact);

        qCDebug(logNetwork) << "PUT" << req.url().toString()
                    << "payload:" << jsonData;

        emit requestStarted();
        QNetworkReply* reply = m_manager->put(req, jsonData);
        sendRequest(reply, callback);
    }

    void HttpClient::del(const QString &endpoint, const JsonCallback& callback) {
        const QNetworkRequest req = createRequest(endpoint);

        qCDebug(logNetwork) << "DELETE" << req.url().toString();

        emit requestStarted();
        QNetworkReply* reply = m_manager->deleteResource(req);
        sendRequest(reply, callback);
    }

    void HttpClient::sendRequest(QNetworkReply* reply, const JsonCallback& callback) {
        connect(reply, &QNetworkReply::finished, this, [this, reply, callback]() {
            handleReply(reply, callback);
            emit requestFinished();
        });
    }

    void HttpClient::handleReply(QNetworkReply *reply, const JsonCallback& callback) {
        // 1. Сначала читаем данные и статус, пока reply жив
        int http_status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QByteArray response_data = reply->readAll();

        // 2. Проверяем, есть ли вообще сетевая ошибка
        if (reply->error() != QNetworkReply::NoError) {

            // СПЕЦИАЛЬНАЯ ОБРАБОТКА ДЛЯ HTTP 4xx/5xx ОШИБОК В QT 6
            // Если статус >= 400, значит сервер ответил, просто с ошибкой валидации.
            // Мы хотим показать пользователю красивый текст из JSON, а не "Network error".
            if (http_status >= 400 && !response_data.isEmpty()) {
                QString error_msg = QString("HTTP Error %1").arg(http_status);

                QJsonParseError parse_err;
                QJsonDocument doc = QJsonDocument::fromJson(response_data, &parse_err);

                if (parse_err.error == QJsonParseError::NoError && doc.isObject()) {
                    QJsonObject obj = doc.object();
                    if (obj.contains("error"))
                        error_msg += ": " + obj["error"].toString();
                    else if (obj.contains("message"))
                        error_msg += ": " + obj["message"].toString();
                } else {
                     error_msg += ": " + QString::fromUtf8(response_data);
                }

                qCWarning(logNetwork) << error_msg;
                callback(QJsonObject(), error_msg);
                reply->deleteLater();
                return;
            }

            // Если это настоящая сетевая ошибка (нет интернета, таймаут, DNS)
            QString error_msg = QString("Network error [%1]: %2")
                                    .arg(reply->error())
                                    .arg(reply->errorString());
            qCWarning(logNetwork) << error_msg;
            callback(QJsonObject(), error_msg);
            reply->deleteLater();
            return;
        }

        // 3. Если сетевой ошибки нет (статус 200-299), работаем как обычно
        qCDebug(logNetwork) << "Response status:" << http_status << "size:" << response_data.size();

        if (response_data.isEmpty()) {
            callback(QJsonObject(), "");
            reply->deleteLater();
            return;
        }

        QJsonParseError parse_error;
        QJsonDocument doc = QJsonDocument::fromJson(response_data, &parse_error);

        if (parse_error.error != QJsonParseError::NoError) {
            QString error_msg = QString("JSON parse error: %1").arg(parse_error.errorString());
            qCCritical(logNetwork) << error_msg;
            callback(QJsonObject(), error_msg);
            reply->deleteLater();
            return;
        }

        if (!doc.isObject()) {
            QString error_msg = "Response is not a JSON object";
            qCCritical(logNetwork) << error_msg;
            callback(QJsonObject(), error_msg);
            reply->deleteLater();
            return;
        }

        qCInfo(logNetwork) << "Successfully parsed response";
        callback(doc.object(), "");

        reply->deleteLater();
    }

} // infra