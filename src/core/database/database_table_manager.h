#ifndef DATABASE_TABLE_MANAGER
#define DATABASE_TABLE_MANAGER

#include <string>
#include <vector>
#include <unordered_map>

#include "database.h"
#include "core/database/schema/table_schema.h"

class DatabaseTableManager {
public:
    DatabaseTableManager(Database* db);
    DatabaseTableManager(const DatabaseTableManager&) = delete;
    DatabaseTableManager& operator=(const DatabaseTableManager&) = delete;

    // Table Operations
    void create_table(const TableSchema& table_schema);
    void create_table(const wxString& table_name);
    void drop_table(const wxString& selected_tb_name);

    void set_current_table_name(const std::string& table_name);
    std::vector<std::string> get_table_names() const;
    std::string get_current_table_name() const;
    TableSchema get_current_table_schema() const;
    // void drop_table(const std::string& table_name);

    // Schema Operations
    
    // Column Operations
    void update_column(int row, const ColumnDefinition &col_def);
    void add_column(const ColumnDefinition& col_def);
    void drop_column(const std::string& col_name);
private:
    std::string current_table_name_;
    Database* db_; // Current Database
    void scan_existing_tables();
    TableSchema fetch_table_schema(const std::string table_name);
    std::unordered_map<std::string, TableSchema> table_schemas_;
};


#endif // DATABASE_TABLE_MANAGER