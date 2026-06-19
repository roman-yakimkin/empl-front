//
// Created by Roman Yakimkin on 11.06.2026.
//

#pragma once
#include "core/contracts/i_department_repo.h"
#include "infrastructure/httpclient.h"
#include "core/cmd/department.h"

namespace repo {
    class Department : public QObject, public core::IDepartmentRepo {
        Q_OBJECT

        infra::HttpClient* m_http_client;
    public:
        explicit Department(infra::HttpClient* http_client, QObject* parent = nullptr)
            : QObject(parent), m_http_client(http_client) {
            if (!m_http_client) throw std::invalid_argument("http client cannot be null");
        };
        ~Department() override = default;

        void getOne(int id, DepartmentCallback callback) override;
        void getAll(DepartmentsCallback callback) override;
        void create(const cmd::department::Create &cmd, DepartmentCallback callback) override;
        void update(const cmd::department::Update &cmd, DepartmentCallback callback) override;
        void del(const cmd::department::Delete &cmd, BoolCallback callback) override;
    };
} // repo