//
// Created by Roman Yakimkin on 16.06.2026.
//

// You may need to build the project (run Qt uic code generator) to get "ui_Settings.h" resolved

#include <QMessageBox>

#include "settings.h"
#include "ui_settings.h"

namespace views {
    Settings::Settings(infra::AppConfig* cfg, QWidget *parent) : QDialog(parent), ui(new Ui::Settings), m_cfg(cfg) {
        ui->setupUi(this);

        loadSettings();

        connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &Settings::onAccepted);
        connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &Settings::onRejected);
        connect(ui->btnReset, &QPushButton::clicked, this, &Settings::onResetClicked);
    }

    Settings::~Settings() {
        delete ui;
    }

    void Settings::loadSettings() {
        ui->leApiUrl->setText(m_cfg->apiBaseUrl());
        ui->spnPort->setValue(m_cfg->apiPort());
        ui->spnTimeout->setValue(m_cfg->apiTimeout());
    }

    void Settings::saveSettings() {
        m_cfg->setApiBaseUrl(ui->leApiUrl->text().trimmed());
        m_cfg->setApiPort(ui->spnPort->value());
        m_cfg->setApiTimeout(ui->spnTimeout->value());
    }

    void Settings::onAccepted() {
        // Валидация перед сохранением
        QString url = ui->leApiUrl->text().trimmed();
        if (url.isEmpty()) {
            QMessageBox::warning(this, tr("Ошибка"), tr("URL не может быть пустым"));
            return;  // Не закрываем диалог
        }

        // Сохраняем и закрываем
        saveSettings();
        accept();
    }

    void Settings::onRejected() {
        reject();
    }

    void Settings::onResetClicked() {
        auto reply = QMessageBox::question(
            this,
            tr("Сброс настроек"),
            tr("Вы уверены, что хотите сбросить все настройки к значениям по умолчанию?"),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
        );

        if (reply == QMessageBox::Yes) {
            m_cfg->resetToDefaults();
            loadSettings();
        }
    }
}
