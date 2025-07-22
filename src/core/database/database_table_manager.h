#ifndef DATABASE_TABLE_MANAGER
#define DATABASE_TABLE_MANAGER

#include <string>
#include <vector>
#include "database.h"
#include "core/database/stmt_builder/table_schema.h"

class DatabaseTableManager {
public:
    DatabaseTableManager(Database* db) : db_(db) {
        scan_existing_tables();
    }
    DatabaseTableManager(const DatabaseTableManager&) = delete;
    DatabaseTableManager& operator=(const DatabaseTableManager&) = delete;

    // Table Operations
    void create_table(const TableSchema& table_schema);
    void create_table(const wxString &table_name);
    void drop_table(const wxString &selected_tb_name);
    std::string get_current_table_name();
    void set_current_table_name();
    // void drop_table(const std::string& table_name);

    // Schema Operations
    
    // Column Operations

    std::vector<std::string> get_table_names() const;
private:
    void scan_existing_tables();
    TableSchema fetch_table_schema(const std::string table_name);
    Database *db_;
    std::vector<TableSchema> table_schemas_;    
};


#endif // DATABASE_TABLE_MANAGER