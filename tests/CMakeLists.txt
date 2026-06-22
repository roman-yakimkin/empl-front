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
