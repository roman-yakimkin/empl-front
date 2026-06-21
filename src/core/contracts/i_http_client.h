//
// Created by Roman Yakimkin on 21.06.2026.
//

#pragma once

#include <QString>
#include <QJsonObject>

using JsonCallback = std::function<void(const QJsonObject& json, const QString& error)>;
using RawCallback = std::function<void(const QByteArray& data, const QString& error)>;

namespace core {
    class IHttpClient {
    public:
        virtual ~IHttpClient() = default;

        virtual void get(const QString& endpoint, const JsonCallback& callback) = 0;
        virtual void post(const QString& endpoint, const QJsonObject& payload, const JsonCallback& callback) = 0;
        virtual void put(const QString& endpoint, const QJsonObject& payload, const JsonCallback& callback) = 0;
        virtual void del(const QString& endpoint, const JsonCallback& callback) = 0;
    };
} // core