cmake_minimum_required(VERSION 3.10.0)
project(hm_system)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH "opt/homebrew/opt")
find_package (SQLite3 REQUIRED)
find_package (wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
add_executable(hm_system
    # ${SRC_DIR}/main.cpp
    ${SRC_DIR}/core/database/database_exception.cpp
    ${SRC_DIR}/core/database/database_manager.cpp
    ${SRC_DIR}/core/database/database_ops_base.cpp
    ${SRC_DIR}/core/database/transaction_manager.cpp
    ${SRC_DIR}/core/database/database_stmt_builder.cpp
    ${SRC_DIR}/core/database/database_file_handler.cpp
    ${SRC_DIR}/core/database/stmt_builder/where_builder.cpp
    ${SRC_DIR}/core/database/stmt_builder/database_stmt_builder_exception.cpp
    ${SRC_DIR}/gui/database_grid_manager.cpp
    ${SRC_DIR}/gui/database_search_manager.cpp
    ${SRC_DIR}/gui/wx_database_grid_panel.cpp
    ${SRC_DIR}/gui/wx_search_panel.cpp
    ${SRC_DIR}/cli/cli_util.cpp
    ${SRC_DIR}/gui/wx_test.cpp
)
target_include_directories(hm_system PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries (hm_system SQLite::SQLite3 ${wxWidgets_LIBRARIES})
