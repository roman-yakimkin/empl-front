//
// Created by Roman Yakimkin on 18.06.2026.
//

#pragma once

#include <QString>
#include <QWidget>

namespace views::common {
    class BasePage : public QWidget {
        Q_OBJECT
    public:
        explicit BasePage(QWidget *parent = nullptr) : QWidget(parent) {}
        ~BasePage() override = default;

        virtual void reloadData() = 0;
    };
} // views