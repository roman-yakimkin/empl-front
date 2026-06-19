# Учет сотрудников (empl-front)

Десктопное приложение для управления сотрудниками, отделами и должностями. Разработано на C++ с использованием фреймворка Qt6.

## 📖 Описание

Приложение предоставляет графический интерфейс для управления справочниками сотрудников, отделов и должностей. Взаимодействует с бэкенд-микросервисом через REST API.

**Бэкенд:** [empl-back](https://github.com/roman-yakimkin/empl-back)

## ✨ Возможности

- **Управление отделами**
  - Создание, редактирование, удаление отделов
  - Просмотр списка отделов в табличном виде

- **Управление должностями**
  - Создание, редактирование, удаление должностей
  - Быстрое добавление прямо из формы сотрудника

- **Управление сотрудниками**
  - Создание, редактирование, удаление сотрудников
  - Привязка к отделам и должностям
  - Отображение дат создания и изменения

- **Настройки подключения**
  - Настройка URL, порта и таймаутов API
  - Сохранение настроек в локальном конфигурационном файле

- **Современный интерфейс**
  - Адаптивный дизайн с разделением на списки и детали
  - Модальные диалоги для подтверждения операций
  - Обработка ошибок с понятными сообщениями

## 🏗️ Архитектура

Приложение построено по трёхслойной архитектуре:

- **UI Layer (Qt Widgets):** DepartmentPage, PositionPage, EmployeePage, Settings
- **Service Layer (Business Logic):** DepartmentSvc, PositionSvc, EmployeeSvc
- **Repository Layer (HTTP Client):** DepartmentRepo, PositionRepo, EmployeeRepo, HttpClient
- **Backend API:** REST API с JSON-ответами ([empl-back](https://github.com/roman-yakimkin/empl-back))

```
    ┌─────────────────────────────────────┐
    │         UI Layer (Qt Widgets)       │
    │  - DepartmentPage                   │
    │  - PositionPage                     │
    │  - EmployeePage                     │
    │  - Settings                         │
    └──────────────┬──────────────────────┘
                   │
    ┌──────────────▼──────────────────────┐
    │      Service Layer (Business Logic) │
    │  - DepartmentSvc                    │
    │  - PositionSvc                      │
    │  - EmployeeSvc                      │
    └──────────────┬──────────────────────┘
                   │
    ┌──────────────▼──────────────────────┐
    │    Repository Layer (HTTP Client)   │
    │  - DepartmentRepo                   │
    │  - PositionRepo                     │
    │  - EmployeeRepo                     │
    │  - HttpClient(QNetworkAccessManager)│
    └──────────────┬──────────────────────┘
                   │
    ┌──────────────▼──────────────────────┐
    │        Backend API (empl-back)      │
    │  - REST API                         │
    │  - JSON responses                   │
    └─────────────────────────────────────┘
```

### Ключевые особенности реализации

- **Dependency Injection** — все зависимости передаются через конструкторы
- **Signals & Slots** — асинхронное взаимодействие между слоями
- **Model-View** — использование `QAbstractTableModel` для таблиц
- **QSettings** — хранение конфигурации приложения
- **Error Handling** — централизованная обработка ошибок сети и валидации

## 🚀 Быстрый старт

### Требования

- **CMake** 3.16 или выше
- **Qt** 6.5 или выше (модули Widgets и Network)
- **C++20** совместимый компилятор (GCC 10+, Clang 10+, MSVC 2019+)
- Запущенный бэкенд-сервер [empl-back](https://github.com/roman-yakimkin/empl-back)

### Установка и запуск

#### Linux (Manjaro/Arch)

```bash
git clone https://github.com/roman-yakimkin/empl-front.git
cd empl-front
sudo pacman -S qt6-base cmake
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
./build/empl_front
```

#### Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install -y qt6-base-dev qt6-network-dev cmake build-essential
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
./build/empl_front
```

#### Windows

```cmd
cmake -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
.\build\Release\empl_front.exe
```

#### macOS

```bash
brew install qt@6
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(sysctl -n hw.ncpu)
./build/empl_front
```

### Настройка подключения

При первом запуске приложение использует настройки по умолчанию:
- **Base URL:** `http://localhost`
- **Port:** `8080`
- **Timeout:** `30 секунд`

Для изменения:
1. Откройте меню **Настройки** (или `Ctrl+,`)
2. Укажите параметры подключения к вашему бэкенду
3. Нажмите **Сохранить**

Файл конфигурации сохраняется в:
- **Linux:** `~/.config/empl-front/empl-front.conf`
- **Windows:** `%APPDATA%\empl-front\empl-front.ini`
- **macOS:** `~/Library/Preferences/com.empl-front.plist`

## 📁 Структура проекта

```
empl-front/
├── src/
│   ├── main.cpp                      # Точка входа
│   ├── ui/                           # UI-слой
│   │   ├── mainwindow.h/cpp          # Главное окно
│   │   ├── departmentpage.h/cpp      # Страница отделов
│   │   ├── positionpage.h/cpp        # Страница должностей
│   │   ├── employeepage.h/cpp        # Страница сотрудников
│   │   ├── settings.h/cpp            # Диалог настроек
│   │   ├── common/
│   │   │   └── basepage.h            # Базовый класс для страниц
│   │   └── models/
│   │       └── table/                # Модели таблиц
│   │           ├── department.h/cpp
│   │           ├── position.h/cpp
│   │           └── employee.h/cpp
│   ├── core/                         # Бизнес-логика
│   │   ├── contracts/                # Интерфейсы сервисов
│   │   │   ├── i_department_svc.h
│   │   │   ├── i_position_svc.h
│   │   │   └── i_employee_svc.h
│   │   ├── models/                   # Модели данных
│   │   │   ├── department.h/cpp
│   │   │   ├── position.h/cpp
│   │   │   └── employee.h/cpp
│   │   └── cmd/                      # Команды (DTO)
│   │       ├── department.h
│   │       ├── position.h
│   │       └── employee.h
│   ├── repositories/                 # Слой работы с данными
│   │   ├── department.h/cpp
│   │   ├── position.h/cpp
│   │   └── employee.h/cpp
│   ├── services/                     # Сервисный слой
│   │   ├── department.h/cpp
│   │   ├── position.h/cpp
│   │   └── employee.h/cpp
│   └── infrastructure/               # Инфраструктура
│       ├── httpclient.h/cpp          # HTTP-клиент
│       ├── appconfig.h/cpp           # Конфигурация
│       └── logging.h/cpp             # Логирование
├── CMakeLists.txt                    # Конфигурация сборки
├── README.md                         # Этот файл
└── LICENSE                           # Лицензия
```

## 🔧 Разработка

### Сборка в Debug-режиме

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Форматирование кода

```bash
find src -name "*.cpp" -o -name "*.h" | xargs clang-format -i
```

## 🐛 Известные ограничения

- Требуется активное подключение к бэкенд-серверу
- Нет оффлайн-режима с локальным кэшированием
- Аутентификация не реализована (планируется)

## 📄 Лицензия

Этот проект распространяется под лицензией MIT. См. файл [LICENSE](LICENSE) для подробностей.

## 📞 Контакты

**Автор:** Roman Yakimkin  
**GitHub:** [@roman-yakimkin](https://github.com/roman-yakimkin)

---

**Примечание:** Это демо-приложение, разработанное в учебных целях. Для продакшн-использования рекомендуется добавить аутентификацию, аудит действий и резервное копирование данных.