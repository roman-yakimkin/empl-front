//
// Created by Roman Yakimkin on 11.06.2026.
//

#pragma once

#include <stdexcept>

#include "core/contracts/i_department_repo.h"
#include "core/contracts/i_department_svc.h"

namespace svc {
    class Department : public core::IDepartmentSvc {
        Q_OBJECT

        core::IDepartmentRepo *m_repo = nullptr;
    public:
        explicit Department(core::IDepartmentRepo* repo, QObject* parent = nullptr) : IDepartmentSvc(parent), m_repo(repo) {
            if (!m_repo) throw std::invalid_argument("department repo cannot be null");
        }
        ~Department() override = default;

        void getOne(int id) override;
        void getAll() override;
        void create(const cmd::department::Create &cmd) override;
        void update(const cmd::department::Update &cmd) override;
        void del(const cmd::department::Delete &cmd) override;
    };

} // svc