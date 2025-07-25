# NOW

Use customiszed grid class

[bug] cant scroll at view panel untill manually resize

wxSchemaManager commit

wxTableManager create new table

wxTableManagerDialog on_ok and on_cacel commit tb alter active, on_select set current db for tb_manager 
    (This might crash program, or not work properly, since tb_manager is just local unique_ptr, dont share across dialogs, change its current database wont work or others)
    
wxViewPanel choice box for select table

Global commit and rollback feature

wxNotepad for multi table opening

Preview Dialog (grab 50 lines of csv construct a temp table)
wxImportDialog

Auto created Hidden Primary key when user don specify primary key

Table_Manager Alter Table



## IMPORTANT
Check if database is open at Database::execute_sql

add nullptr check at all possible
restruct my code structure. My manager classes and inherit structure is a mess.

# FUTURE

Rename Listbox::list_panel_ to list_box_

Start a transaction when manager_dialog showup, press cancel -> rollback, press ok -> commit
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

