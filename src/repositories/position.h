//
// Created by Roman Yakimkin on 11.06.2026.
//

#pragma once

#include "core/contracts/i_position_repo.h"
#include "infrastructure/httpclient.h"
#include "core/cmd/position.h"

namespace repo {
    class Position : public QObject, public core::IPositionRepo {
        Q_OBJECT

        infra::HttpClient* m_http_client;
    public:
        explicit Position(infra::HttpClient* http_client, QObject* parent = nullptr)
            : QObject(parent), m_http_client(http_client) {
            if (!m_http_client) throw std::invalid_argument("http client cannot be null");
        };
        ~Position() override = default;

        void getOne(int id, PositionCallback callback) override;
        void getAll(PositionsCallback callback) override;
        void create(const cmd::position::Create &cmd, PositionCallback callback) override;
        void update(const cmd::position::Update &cmd, PositionCallback callback) override;
        void del(const cmd::position::Delete &cmd, BoolCallback callback) override;

    };
} // repo