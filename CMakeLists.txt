cmake_minimum_required(VERSION 3.10.0)
project(hm_system)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH "opt/homebrew/opt")
find_package (SQLite3 REQUIRED)
find_package (wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
# TODO:
# try to use target_sources and sources.cmake
# https://chatgpt.com/c/687761df-524c-8011-979c-707ef136e524
add_executable(hm_system
    # ${SRC_DIR}/main.cpp
    ${SRC_DIR}/core/database/database.cpp
    ${SRC_DIR}/core/database/database_manager.cpp
    ${SRC_DIR}/core/database/database_exception.cpp
    ${SRC_DIR}/core/database/database_ops_base.cpp
    ${SRC_DIR}/core/database/transaction_manager.cpp
    ${SRC_DIR}/core/database/database_stmt_builder.cpp
    ${SRC_DIR}/core/database/database_file_handler.cpp
    ${SRC_DIR}/core/database/stmt_builder/where_builder.cpp
    ${SRC_DIR}/core/database/stmt_builder/database_stmt_builder_exception.cpp
    ${SRC_DIR}/core/database/database_search_manager.cpp
    ${SRC_DIR}/core/database/database_table_manager.cpp

    ${SRC_DIR}/gui/gui_components/labeled_text_ctrl.cpp
    ${SRC_DIR}/gui/gui_components/listbox.cpp

    ${SRC_DIR}/gui/dialog/wx_base_dialog.cpp
    ${SRC_DIR}/gui/dialog/wx_database_manager_dialog.cpp
    ${SRC_DIR}/gui/dialog/wx_get_folder_path_dialog.cpp
    ${SRC_DIR}/gui/database_grid_manager.cpp
    ${SRC_DIR}/gui/wx_database_grid_panel.cpp
    ${SRC_DIR}/gui/wx_search_panel.cpp
    # ${SRC_DIR}/gui/wx_test.cpp
    ${SRC_DIR}/cli/cli_util.cpp
    ${SRC_DIR}/hr_system.cpp
)
target_include_directories(hm_system PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries (hm_system SQLite::SQLite3 ${wxWidgets_LIBRARIES})
