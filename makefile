# Makefile — удобная обёртка над CMake/CTest для проекта empl-front

BUILD_DIR := build
CMAKE := cmake
CTEST := ctest
JOBS := $(shell nproc)

.PHONY: help all build test test-verbose test-one clean reconfigure

# по умолчанию - справка по доступным командам
help:
	@echo ""
	@echo "Доступные команды:"
	@echo ""
	@echo "  make all              - Собрать проект"
	@echo "  make test             - Пересобрать и запустить все тесты"
	@echo "  make test-verbose     - Запустить тесты с подробным выводом"
	@echo "  make test-one TEST=<имя> - Запустить конкретный тест"
	@echo "                          Пример: make test-one TEST=test_department_model"
	@echo "  make clean            - Удалить директорию сборки"
	@echo "  make reconfigure      - Полная пересборка (очистка + сборка)"
	@echo "  make coverage         - Запустить тесты (подготовка для отчёта покрытия)"
	@echo "  make help             - Показать эту справку (по умолчанию)"
	@echo ""
	@echo "💡 Примеры использования:"
	@echo "  make test                          # Запустить все тесты"
	@echo "  make test-one TEST=test_employee   # Запустить один тест"
	@echo "  make clean && make test            # Чистая сборка и тесты"
	@echo ""

# Сборка проекта
all: build

# Конфигурация проекта (вызывать только при первом запуске или изменении CMakeLists.txt)
configure:
	@echo "🔧 Конфигурация проекта..."
	@$(CMAKE) -B $(BUILD_DIR) -S .
	@echo "✅ Конфигурация завершена"

# Сборка проекта
build: configure
	@echo "🔨 Сборка проекта..."
	@$(CMAKE) --build $(BUILD_DIR) -j$(JOBS)
	@echo "✅ Сборка завершена"

# Запуск всех тестов (с автоматической пересборкой)
test: build
	@echo "🧪 Запуск тестов..."
	@cd $(BUILD_DIR) && $(CTEST) --output-on-failure
	@echo "✅ Все тесты пройдены"

# Запуск тестов с подробным выводом
test-verbose: build
	@echo "🧪 Запуск тестов (подробный вывод)..."
	@cd $(BUILD_DIR) && $(CTEST) -V
	@echo "✅ Все тесты пройдены"

# Запуск конкретного теста по имени
# Использование: make test-one TEST=test_department_model
test-one: build
ifndef TEST
	@echo "❌ Использование: make test-one TEST=<имя_теста>"
	@echo "   Пример: make test-one TEST=test_department_model"
	@exit 1
endif
	@echo "🧪 Запуск теста: $(TEST)"
	@$(BUILD_DIR)/tests/$(TEST)

# Очистка директории сборки
clean:
	@echo "🧹 Очистка директории сборки..."
	@rm -rf $(BUILD_DIR)
	@echo "✅ Очистка завершена"

# Полная пересборка (очистка + сборка)
reconfigure: clean build
	@echo "✅ Полная пересборка завершена"

# Запуск тестов с генерацией отчёта о покрытии кода (если настроено)
coverage: build
	@echo "📊 Генерация отчёта о покрытии кода..."
	@cd $(BUILD_DIR) && $(CTEST) --output-on-failure
	@echo "⚠️  Для генерации отчёта покрытия требуется дополнительная настройка CMake"
	@echo "   Смотри документацию по gcov/lcov"
