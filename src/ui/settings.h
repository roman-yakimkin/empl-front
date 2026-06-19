//
// Created by Roman Yakimkin on 16.06.2026.
//

#pragma once

#include <QDialog>

#include "infrastructure/appconfig.h"
#include "infrastructure/httpclient.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class Settings;
}

QT_END_NAMESPACE

namespace views {
    class Settings : public QDialog {
        Q_OBJECT

    public:
        explicit Settings(infra::AppConfig* cfg, QWidget *parent = nullptr);

        ~Settings() override;

    private slots:
        void onAccepted();
        void onRejected();
        void onResetClicked();

    private:
        Ui::Settings *ui;
        infra::AppConfig* m_cfg;

        void loadSettings();
        void saveSettings();
    };
}
