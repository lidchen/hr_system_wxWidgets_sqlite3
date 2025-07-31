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

    # CLI
    ${SRC_DIR}/cli/cli_util.cpp

    # DATABASE
    ${SRC_DIR}/core/database/database.cpp
    ${SRC_DIR}/core/database/database_manager.cpp
    ${SRC_DIR}/core/database/database_exception.cpp
    ${SRC_DIR}/core/database/database_stmt_builder.cpp
    ${SRC_DIR}/core/database/stmt_builder/database_stmt_builder_exception.cpp
    ${SRC_DIR}/core/database/database_table_manager.cpp
    ${SRC_DIR}/core/database/stmt_builder/where_builder.cpp
    # ${SRC_DIR}/core/database/database_search_manager.cpp

    # SCHEMA
    ${SRC_DIR}/core/database/schema/table_schema.cpp
    ${SRC_DIR}/core/database/schema/column_definition.cpp

    # GUI COMPONENTS
    ${SRC_DIR}/gui/gui_components/labeled_text_ctrl.cpp
    ${SRC_DIR}/gui/gui_components/listbox.cpp
    ${SRC_DIR}/gui/gui_components/cb_select_popup.cpp
    ${SRC_DIR}/gui/gui_components/smart_grid.cpp
    ${SRC_DIR}/gui/gui_components/grid_panel/wx_editable_grid_panel.cpp
    ${SRC_DIR}/gui/gui_components/grid_panel/wx_table_schema_grid_panel.cpp
    ${SRC_DIR}/gui/gui_components/grid_panel/wx_database_editor_grid_panel.cpp
    ${SRC_DIR}/gui/gui_components/search_panel/basic_search_panel.cpp
    ${SRC_DIR}/gui/gui_components/search_panel/advance_search_panel.cpp
    ${SRC_DIR}/gui/gui_components/search_panel/wx_search_panel.cpp

    # DIALOG
    ${SRC_DIR}/gui/dialog/wx_base_dialog.cpp
    ${SRC_DIR}/gui/dialog/wx_database_manager_dialog.cpp
    ${SRC_DIR}/gui/dialog/wx_get_folder_path_dialog.cpp
    ${SRC_DIR}/gui/dialog/wx_get_file_path_dialog.cpp
    ${SRC_DIR}/gui/dialog/wx_table_manager_dialog.cpp
    ${SRC_DIR}/gui/dialog/wx_table_schema_dialog.cpp
    ${SRC_DIR}/gui/dialog/wx_create_table_dialog.cpp
    # TODO
    # ${SRC_DIR}/gui/dialog/wx_import_dialog.cpp

    # GUI
    ${SRC_DIR}/gui/database_editor_panel.cpp
    ${SRC_DIR}/gui/welcome_panel.cpp
    ${SRC_DIR}/gui/main_frame.cpp
    ${SRC_DIR}/hr_system.cpp
    # ${SRC_DIR}/main.cpp
    # ${SRC_DIR}/gui/wx_test.cpp

)
target_include_directories(hm_system PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries (hm_system SQLite::SQLite3 ${wxWidgets_LIBRARIES})