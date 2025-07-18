# NOW
database set current database function
bind database manager dialog's ok btn to set current database function



# FUTURE

replace all raw pointer to smart ptr (exclude wx native class, they should be raw)

database_ops_base.cpp
    Split DatabaseOpsBase::execute_sql function into smaller ones

base wxPanel for creation(database, table, user...)

make labeled_text_ctrl inherit from horzontal_panel

merge database_file_hander into database_manager

Dynamic Table Creation
    wxCreateTablePanel
    TableSet/AlterPropertiesPanel
        Name
        column type
        Properties checkbox (according to column type) -> searchable, displayable...
        create default search type class

Dynamic Search