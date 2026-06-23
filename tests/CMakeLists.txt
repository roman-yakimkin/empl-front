# Вспомогательная функция для добавления теста
function(add_qt_test TEST_NAME TEST_SOURCE)
    add_executable(${TEST_NAME}
            ${TEST_SOURCE}
            ${ARGN}           # все дополнительные аргументы после TEST_SOURCE
    )
    target_link_libraries(${TEST_NAME} PRIVATE
            Qt6::Core
            Qt6::Test
            Qt6::Gui
            Qt6::Widgets
            Qt6::Network
    )
    target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

add_qt_test(test_department_model models/test_department_model.cpp)
add_qt_test(test_position_model models/test_position_model.cpp)
add_qt_test(test_employee_model models/test_employee_model.cpp ${CMAKE_SOURCE_DIR}/src/core/models/employee.cpp)

add_qt_test(test_department_repo
        repositories/test_department_repo.cpp
        ${CMAKE_SOURCE_DIR}/src/repositories/department.cpp
        ${CMAKE_SOURCE_DIR}/src/infrastructure/logging.cpp
)
add_qt_test(test_position_repo
        repositories/test_position_repo.cpp
        ${CMAKE_SOURCE_DIR}/src/repositories/position.cpp
        ${CMAKE_SOURCE_DIR}/src/infrastructure/logging.cpp
)
add_qt_test(test_employee_repo
        repositories/test_employee_repo.cpp
        ${CMAKE_SOURCE_DIR}/src/core/models/employee.cpp
        ${CMAKE_SOURCE_DIR}/src/repositories/employee.cpp
        ${CMAKE_SOURCE_DIR}/src/infrastructure/logging.cpp
)

add_qt_test(test_department_svc
        services/test_department_svc.cpp
        mocks/mock_department_repo.h
        ${CMAKE_SOURCE_DIR}/src/core/contracts/i_department_svc.cpp
        ${CMAKE_SOURCE_DIR}/src/services/department.cpp
        ${CMAKE_SOURCE_DIR}/src/infrastructure/logging.cpp
)
add_qt_test(test_position_svc
        services/test_position_svc.cpp
        mocks/mock_position_repo.h
        ${CMAKE_SOURCE_DIR}/src/core/contracts/i_position_svc.cpp
        ${CMAKE_SOURCE_DIR}/src/services/position.cpp
        ${CMAKE_SOURCE_DIR}/src/infrastructure/logging.cpp
)
add_qt_test(test_employee_svc
        services/test_employee_svc.cpp
        mocks/mock_employee_repo.h
        ${CMAKE_SOURCE_DIR}/src/core/models/employee.cpp
        ${CMAKE_SOURCE_DIR}/src/core/contracts/i_employee_svc.cpp
        ${CMAKE_SOURCE_DIR}/src/services/employee.cpp
        ${CMAKE_SOURCE_DIR}/src/infrastructure/logging.cpp
)

add_qt_test(test_department_page
        ui/test_department_page.cpp
        mocks/mock_department_svc.h
        ${CMAKE_SOURCE_DIR}/src/ui/common/basepage.h
        ${CMAKE_SOURCE_DIR}/src/ui/departmentpage.h
        ${CMAKE_SOURCE_DIR}/src/ui/departmentpage.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/models/table/department.h
        ${CMAKE_SOURCE_DIR}/src/ui/models/table/department.cpp
        ${CMAKE_SOURCE_DIR}/src/core/contracts/i_department_svc.cpp
        ${CMAKE_SOURCE_DIR}/src/services/department.cpp
        ${CMAKE_SOURCE_DIR}/src/infrastructure/logging.cpp
)
add_qt_test(test_position_page
        ui/test_position_page.cpp
        mocks/mock_position_svc.h
        ${CMAKE_SOURCE_DIR}/src/ui/common/basepage.h
        ${CMAKE_SOURCE_DIR}/src/ui/positionpage.h
        ${CMAKE_SOURCE_DIR}/src/ui/positionpage.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/models/table/position.h
        ${CMAKE_SOURCE_DIR}/src/ui/models/table/position.cpp
        ${CMAKE_SOURCE_DIR}/src/core/contracts/i_position_svc.cpp
        ${CMAKE_SOURCE_DIR}/src/services/position.cpp
        ${CMAKE_SOURCE_DIR}/src/infrastructure/logging.cpp
)
add_qt_test(test_employee_page
        ui/test_employee_page.cpp
        mocks/mock_department_svc.h
        mocks/mock_position_svc.h
        mocks/mock_employee_svc.h
        ${CMAKE_SOURCE_DIR}/src/ui/common/basepage.h
        ${CMAKE_SOURCE_DIR}/src/ui/employeepage.h
        ${CMAKE_SOURCE_DIR}/src/ui/employeepage.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/models/table/employee.h
        ${CMAKE_SOURCE_DIR}/src/ui/models/table/employee.cpp
        ${CMAKE_SOURCE_DIR}/src/core/contracts/i_department_svc.cpp
        ${CMAKE_SOURCE_DIR}/src/core/contracts/i_position_svc.cpp
        ${CMAKE_SOURCE_DIR}/src/core/contracts/i_employee_svc.cpp
        ${CMAKE_SOURCE_DIR}/src/services/department.cpp
        ${CMAKE_SOURCE_DIR}/src/services/position.cpp
        ${CMAKE_SOURCE_DIR}/src/services/employee.cpp
        ${CMAKE_SOURCE_DIR}/src/infrastructure/logging.cpp
)

add_qt_test(test_main_window
        ui/test_main_window.cpp
        mocks/mock_configurable_client.h
        mocks/mock_department_svc.h
        mocks/mock_position_svc.h
        mocks/mock_employee_svc.h

        ${CMAKE_SOURCE_DIR}/src/core/contracts/i_configurable_client.h
        ${CMAKE_SOURCE_DIR}/src/core/contracts/i_department_svc.h
        ${CMAKE_SOURCE_DIR}/src/core/contracts/i_department_svc.cpp
        ${CMAKE_SOURCE_DIR}/src/core/contracts/i_position_svc.h
        ${CMAKE_SOURCE_DIR}/src/core/contracts/i_position_svc.cpp
        ${CMAKE_SOURCE_DIR}/src/core/contracts/i_employee_svc.h
        ${CMAKE_SOURCE_DIR}/src/core/contracts/i_employee_svc.cpp

        ${CMAKE_SOURCE_DIR}/src/ui/common/basepage.h
        ${CMAKE_SOURCE_DIR}/src/ui/mainwindow.h
        ${CMAKE_SOURCE_DIR}/src/ui/mainwindow.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/departmentpage.h
        ${CMAKE_SOURCE_DIR}/src/ui/departmentpage.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/positionpage.h
        ${CMAKE_SOURCE_DIR}/src/ui/positionpage.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/employeepage.h
        ${CMAKE_SOURCE_DIR}/src/ui/employeepage.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/settings.h
        ${CMAKE_SOURCE_DIR}/src/ui/settings.cpp

        # Модели таблиц
        ${CMAKE_SOURCE_DIR}/src/ui/models/table/department.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/models/table/position.cpp
        ${CMAKE_SOURCE_DIR}/src/ui/models/table/employee.cpp

        ${CMAKE_SOURCE_DIR}/src/infrastructure/appconfig.cpp

        ${CMAKE_SOURCE_DIR}/src/services/department.cpp
        ${CMAKE_SOURCE_DIR}/src/services/position.cpp
        ${CMAKE_SOURCE_DIR}/src/services/employee.cpp

        ${CMAKE_SOURCE_DIR}/src/infrastructure/logging.cpp
)
