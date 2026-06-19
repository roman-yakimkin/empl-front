//
// Created by Roman Yakimkin on 11.06.2026.
//

#include <cstdio>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QMutexLocker>
#include <QString>
#include <QTextStream>

#include "logging.h"

Q_LOGGING_CATEGORY(logNetwork, "app.network")
Q_LOGGING_CATEGORY(logApp, "app.core")
Q_LOGGING_CATEGORY(logUI, "app.ui")

namespace infra {
    LogFile &LogFile::instance() {
        static LogFile instance;

        return instance;
    }

    void LogFile::initialize(const QString& file_name) {
        if (m_initialized) {
            qCWarning(logApp) << "LogFile already initialized";
            return;
        }

        // Создаём папку для логов
        QString log_dir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir().mkpath(log_dir);

        // Открываем файл
        m_log_file = std::make_unique<QFile>(log_dir + "/" + file_name);
        if (m_log_file->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            qWarning() << "Failed to open log file:" << m_log_file->fileName();
            return;
        }

        m_stream.setDevice(m_log_file.get());

        // Сохраняем предыдущий handler (RAII)
        m_previous_handler = qInstallMessageHandler(LogFile::messageHandler);
        m_initialized = true;

        qCInfo(logApp) << "=== Application started ===";
    }

    LogFile::~LogFile() {
        if (m_initialized) {
            qCInfo(logApp) << "=== Application shutting down ===";

            // Восстанавливаем предыдущий handler
            qInstallMessageHandler(m_previous_handler);

            m_stream.flush();
            m_stream.setDevice(nullptr);
            m_log_file->close();
        }
    }

    void LogFile::messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
        LogFile& self = instance();

        // Форматируем время
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

        // Определяем уровень
        QString level;
        switch (type) {
            case QtDebugMsg:    level = "DEBUG"; break;
            case QtInfoMsg:     level = "INFO "; break;
            case QtWarningMsg:  level = "WARN "; break;
            case QtCriticalMsg: level = "CRIT "; break;
            case QtFatalMsg:    level = "FATAL"; break;
        }

        // Формируем строку лога
        QString logEntry = QString("[%1] [%2] [%3] %4\n")
                               .arg(timestamp)
                               .arg(level, -5)
                               .arg(context.category ? context.category : "default")
                               .arg(msg);

        // Пишем в файл с блокировкой
        QMutexLocker locker(&self.m_mx);
        if (self.m_log_file && self.m_log_file->isOpen()) {
            self.m_stream << logEntry;
            self.m_stream.flush();
        }

        // Дублируем в консоль (опционально, но удобно при разработке)
        if (type == QtFatalMsg) {
            fprintf(stderr, "%s\n", qPrintable(logEntry));
            abort();
        }

        // Опционально: пробрасываем в предыдущий handler
        if (self.m_previous_handler) {
            self.m_previous_handler(type, context, msg);
        }
    }

}
