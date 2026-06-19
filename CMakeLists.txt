cmake_minimum_required(VERSION 4.2)
project(empl_front)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        REQUIRED)

add_executable(empl_front src/main.cpp
        src/core/models/employee.cpp
        src/repositories/department.cpp
        src/repositories/employee.cpp
        src/repositories/position.cpp
        src/services/department.cpp
        src/services/position.cpp
        src/services/employee.cpp
        src/ui/mainwindow.cpp
        src/ui/mainwindow.ui
        src/infrastructure/logging.cpp
        src/infrastructure/appconfig.cpp
        src/infrastructure/httpclient.cpp
        src/ui/departmentpage.cpp
        src/ui/departmentpage.ui
        src/ui/models/table/department.cpp
        src/core/contracts/i_department_svc.cpp
        src/ui/settings.cpp
        src/ui/settings.ui
        src/core/contracts/i_position_svc.cpp
        src/ui/models/table/position.cpp
        src/ui/positionpage.cpp
        src/ui/positionpage.ui
        src/core/contracts/i_employee_svc.cpp
        src/ui/models/table/employee.cpp
        src/ui/employeepage.cpp
        src/ui/employeepage.ui
        src/ui/common/basepage.h
)

target_link_libraries(empl_front
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
)

target_include_directories(empl_front PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

