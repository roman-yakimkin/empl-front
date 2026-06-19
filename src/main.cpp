#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QString>

#include "infrastructure/appconfig.h"
#include "infrastructure/logging.h"
#include "repositories/department.h"
#include "repositories/employee.h"
#include "repositories/position.h"
#include "services/department.h"
#include "services/employee.h"
#include "services/position.h"
#include "ui/mainwindow.h"

int main(int argc, char *argv[]) {
    try {
        QApplication app(argc, argv);

        QCoreApplication::setOrganizationName("Yakimkin Roman");
        QCoreApplication::setApplicationName("Empl qt");
        QCoreApplication::setApplicationVersion("1.0.0");

        // Инициализация конфига
        infra::AppConfig config;

        // Инициализация файла для логирования
        infra::LogFile::instance().initialize("application.log");

        // Инициализация http клиента
        infra::HttpClient http_client(
            config.apiBaseUrl(),
            config.apiPort(),
            config.apiTimeout()
        );

        // Инициализация репозиториев
        repo::Department dep_repo(&http_client);
        repo::Position pos_repo(&http_client);
        repo::Employee empl_repo(&http_client);

        // Инициализация сервисов
        svc::Department dep_svc(&dep_repo);
        svc::Position pos_svc(&pos_repo);
        svc::Employee empl_svc(&empl_repo);

        // Создание и запуск главного окна
        views::MainWindow main_window(&dep_svc, &pos_svc, &empl_svc, &config, &http_client);
        main_window.show();

        return QApplication::exec();
    }
    catch (const std::exception& e) {
        qCritical() << "Fatal error: " << e.what();
        QMessageBox::critical(nullptr, "Error",
        QString("Application failed to start:\n%1").arg(e.what()));

        return -1;
    }
}