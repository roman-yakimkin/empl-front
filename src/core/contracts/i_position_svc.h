//
// Created by Roman Yakimkin on 11.06.2026.
//

#pragma once

#include <QObject>

#include "core/models/position.h"
#include "core/cmd/position.h"

namespace core {
    class IPositionSvc : public QObject {
        Q_OBJECT
    public:
        explicit IPositionSvc(QObject *parent = nullptr) : QObject(parent) {}
        ~IPositionSvc() override;

        virtual void getOne(int id) = 0;
        virtual void getAll() = 0;
        virtual void create(const cmd::position::Create& cmd) = 0;
        virtual void update(const cmd::position::Update& cmd) = 0;
        virtual void del(const cmd::position::Delete& cmd) = 0;

    signals:
        void positionsLoaded(const QVector<model::Position> &items);
        void positionLoaded(const model::Position& item);
        void positionCreated(const model::Position& item);
        void positionUpdated(const model::Position& item);
        void positionDeleted(int id);

        void errorOccurred(const QString &code, const QString &message);
    };
} // core