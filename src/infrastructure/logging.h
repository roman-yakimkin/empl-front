//
// Created by Roman Yakimkin on 11.06.2026.
//

#pragma once

#include <QLoggingCategory>
#include <QMutex>
#include <QFile>
#include <QApplication>

Q_DECLARE_LOGGING_CATEGORY(logNetwork);
Q_DECLARE_LOGGING_CATEGORY(logApp);
Q_DECLARE_LOGGING_CATEGORY(logUI);

namespace infra {
    class LogFile {
    public:
        static LogFile& instance();
        void initialize(const QString& file_name);

        LogFile(const LogFile&) = delete;
        LogFile& operator=(const LogFile&) = delete;

        ~LogFile();
    private:
        std::unique_ptr<QFile> m_log_file;
        QTextStream m_stream;
        QMutex m_mx;
        QtMessageHandler m_previous_handler = nullptr;
        bool m_initialized = false;

        LogFile() = default;

        static void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);
    };
}

