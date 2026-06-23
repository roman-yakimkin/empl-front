//
// Created by Roman Yakimkin on 23.06.2026.
//
#pragma once

#include <QObject>
#include "core/contracts/i_configurable_client.h"

class MockConfigurableClient : public QObject, public core::IConfigurableClient {
    Q_OBJECT
public:
    explicit MockConfigurableClient(QObject *parent = nullptr) {}
    ~MockConfigurableClient() override = default;

    void setBaseUrl(const QString& url) override { base_url = url; }
    void setPort(int port) override { port_val = port; }
    void setTransferTimeout(int timeout_ms) override { timeout = timeout_ms; }

    // Переменные для проверок в тестах (если понадобятся)
    QString base_url;
    int port_val = 0;
    int timeout = 0;
};