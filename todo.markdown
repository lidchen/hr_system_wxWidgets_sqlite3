# NOW

## IMPORTANT
    Check if database is open at Database::execute_sql

    add nullptr check at all possible
    restruct my code structure. My manager classes and inherit structure is a mess.
Dynamic table manager

# FUTURE
Start a transaction when dialog showup, press cancel -> rollback, press ok -> commit
DatabaseException split into error type and msg, for wxErrorLog (dont log error type for better user interface)

Hashmap for table manager and database manager delete and select function

replace all raw pointer to smart ptr (exclude wx native class, they should be raw)

database.cpp
    Split Database::execute_sql function into smaller ones

base wxPanel for creation(database, table, user...)

make labeled_text_ctrl inherit from horzontal_panel

Dynamic Table Creation
    wxCreateTablePanel
    TableSet/AlterPropertiesPanel
        Name
        column type
        Properties checkbox (according to column type) -> searchable, displayable...
        create default search type class

Dynamic Search