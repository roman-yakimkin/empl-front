//
// Created by Roman Yakimkin on 15.06.2026.
//

#pragma once

namespace msg::err {
    constexpr auto ERR_LOAD_DEPARTMENT          = "loadDepartment";
    constexpr auto ERR_LOAD_ALL_DEPARTMENTS     = "loadAllDepartments";
    constexpr auto ERR_CREATE_DEPARTMENT        = "createDepartment";
    constexpr auto ERR_UPDATE_DEPARTMENT        = "updateDepartment";
    constexpr auto ERR_DELETE_DEPARTMENT        = "deleteDepartment";

    constexpr auto ERR_LOAD_POSITION            = "loadPosition";
    constexpr auto ERR_LOAD_ALL_POSITIONS       = "loadAllPositions";
    constexpr auto ERR_CREATE_POSITION          = "createPosition";
    constexpr auto ERR_UPDATE_POSITION          = "updatePosition";
    constexpr auto ERR_DELETE_POSITION          = "deletePosition";

    constexpr auto ERR_LOAD_EMPLOYEE            = "loadEmployee";
    constexpr auto ERR_LOAD_ALL_EMPLOYEES       = "loadAllEmployees";
    constexpr auto ERR_CREATE_EMPLOYEE          = "createEmployee";
    constexpr auto ERR_UPDATE_EMPLOYEE          = "updateEmployee";
    constexpr auto ERR_DELETE_EMPLOYEE          = "deleteEmployee";
}