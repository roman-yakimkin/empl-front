//
// Created by Roman Yakimkin on 11.06.2026.
//

#pragma once
#include <stdexcept>

#include "core/contracts/i_employee_repo.h"
#include "core/contracts/i_employee_svc.h"

namespace svc {
    class Employee : public core::IEmployeeSvc {
        Q_OBJECT

        core::IEmployeeRepo *m_repo = nullptr;
    public:
        explicit Employee(core::IEmployeeRepo* repo, QObject* parent = nullptr) : IEmployeeSvc(parent), m_repo(repo) {
            if (!m_repo) throw std::invalid_argument("employee repo cannot be null");
        }
        ~Employee() override = default;

        void getOne(int id) override;
        void getAll() override;
        void create(const cmd::employee::Create &cmd) override;
        void update(const cmd::employee::Update &cmd) override;
        void del(const cmd::employee::Delete &cmd) override;
    };
} // svc