//
// Created by Roman Yakimkin on 11.06.2026.
//

#pragma once
#include <stdexcept>

#include "core/contracts/i_position_repo.h"
#include "core/contracts/i_position_svc.h"

namespace svc {
    class Position : public core::IPositionSvc {
        Q_OBJECT

        core::IPositionRepo *m_repo = nullptr;
    public:
        explicit Position(core::IPositionRepo* repo, QObject* parent = nullptr) : IPositionSvc(parent), m_repo(repo) {
            if (!m_repo) throw std::invalid_argument("position repo cannot be null");
        }
        ~Position() override = default;

        void getOne(int id) override;
        void getAll() override;
        void create(const cmd::position::Create &cmd) override;
        void update(const cmd::position::Update &cmd) override;
        void del(const cmd::position::Delete &cmd) override;
    };
} // svc