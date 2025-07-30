# TODO
tb_manager_dlg advance search
import_dlg

auto manage col width(make sure use all space)

Global commit and rollback feature
    eg: Start a transaction when manager_dialog showup, press cancel -> rollback, press ok -> commit

wxNotepad for multi table opening

Preview Dialog (grab 50 lines of csv construct a temp table)
wxImportDialog


## IMPORTANT
sql injection & syntax error
https://chatgpt.com/c/68834bb9-18a4-8011-8318-4a53987fabc1

Auto created Hidden Primary key when user don specify primary key

Check if database is open at Database::execute_sql

add nullptr check at all possible
restruct my code structure. My manager classes and inherit structure is a mess.

# FUTURE

database_table_manger all function pass std::string (not wxstring)

[bug] cant scroll at view panel untill manually resize --fixed it in pretty idiotic way

stream-based sql stmt builder

Rename Listbox::list_panel_ to list_box_

DatabaseException split into error type and msg, for wxErrorLog (dont log error type for better user interface)

Hashmap for table manager and database manager delete and select function

replace all raw pointer to smart ptr (exclude wx native class, they should be raw)

database.cpp
    Split Database::execute_sql function into smaller ones

base wxPanel for creation(database, table, user...)

make labeled_text_ctrl inherit from horzontal_panel

Dynamic Search

    | DONE
    1. make column_definition cpp file
    2. implement validate function
    3. use that validate function at view_panel for on_cell_change
    4. on_cell_change 
    https://chatgpt.com/c/68804be7-46ec-8011-b503-ff7d8f2c009f

