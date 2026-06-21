//
// Created by Roman Yakimkin on 21.06.2026.
//

#pragma once

#include "core/contracts/i_http_client.h"

class MockHttpClient : public core::IHttpClient {
public:
    // Данные, которые вернёт мок
    QJsonObject response_to_return;
    QString error_to_return;

    // Для проверки вызовов
    QString last_endpoint;
    QJsonObject last_payload;

    int get_call_count = 0;
    int post_call_count = 0;
    int put_call_count = 0;
    int del_call_count = 0;

    void get(const QString& endpoint, const JsonCallback& callback) override {
        last_endpoint = endpoint;
        get_call_count++;

        callback(response_to_return, error_to_return);
    }

    void post(const QString &endpoint, const QJsonObject &payload, const JsonCallback &callback) override {
        last_endpoint = endpoint;
        last_payload = payload;
        post_call_count++;

        callback(response_to_return, error_to_return);
    };

    void put(const QString &endpoint, const QJsonObject &payload, const JsonCallback &callback) override {
        last_endpoint = endpoint;
        last_payload = payload;
        put_call_count++;

        callback(response_to_return, error_to_return);
    };

    void del(const QString &endpoint, const JsonCallback &callback) override {
        last_endpoint = endpoint;
        del_call_count++;

        callback(response_to_return, error_to_return);
    };
};
