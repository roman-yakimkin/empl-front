//
// Created by Roman Yakimkin on 11.06.2026.
//

#pragma once

#include "core/contracts/i_employee_repo.h"
#include "core/contracts/i_http_client.h"

namespace repo {
    class Employee : public QObject, public core::IEmployeeRepo {
        Q_OBJECT

        core::IHttpClient* m_http_client;
    public:
        explicit Employee(core::IHttpClient* http_client, QObject* parent = nullptr)
            : QObject(parent), m_http_client(http_client) {
            if (!m_http_client) throw std::invalid_argument("http client cannot be null");
        };
        ~Employee() override = default;

        void getOne(int id, EmployeeCallback callback) override;
        void getAll(EmployeesCallback callback) override;
        void create(const cmd::employee::Create &cmd, EmployeeCallback callback) override;
        void update(const cmd::employee::Update &cmd, EmployeeCallback callback) override;
        void del(const cmd::employee::Delete &cmd, BoolCallback callback) override;
    };
} // repo