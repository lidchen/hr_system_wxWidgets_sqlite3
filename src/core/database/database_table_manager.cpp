#include "database_table_manager.h"
#include "cli/cli_util.h"

void DatabaseTableManager::scan_existing_tables() {
    try {
        std::string sql = "SELECT name FROM sqlite_master WHERE type = 'table' AND name NOT LIKE 'sqlite_%'";
        db_->execute_sql(sql, [this](const std::vector<std::string>& col_names, const std::vector<std::string> row_values){
            std::string table_name = row_values[0];
            table_schemas_.push_back(fetch_table_schema(table_name));
        });
    } catch (DatabaseException e) {
        std::cerr << "Failded to scan tables" << e.what() << "\n";
    }
}

// FOR TEST
void DatabaseTableManager::create_table(const wxString& table_name) {
    // DatabaseStmtBuilder builder;
    // builder.create_table(std::string(table_name.mb_str()));
    // auto sql = builder.build();
    std::string sql = "CREATE TABLE " + std::string(table_name.mb_str());
    sql += "(testcol1 TEXT, testcol2 TEXT)";
    db_->execute_sql(sql);
}

void DatabaseTableManager::create_table(const TableSchema& schema) {
    // TODO: 
    // add a schema.is_valid() function
    // add table_exists(schema.name) function
    std::string sql = schema.build_sql();
    db_->execute_sql(sql);
    table_schemas_.push_back(schema);
}

void DatabaseTableManager::drop_table(const wxString& selected_tb_name) {
    std::string sql = "DROP TABLE " + std::string(selected_tb_name.mb_str());
    db_->execute_sql(sql);
    for (auto it = table_schemas_.begin(); it != table_schemas_.end(); ) {
        if (it->table_name_ == std::string(selected_tb_name.mb_str())) {
            it = table_schemas_.erase(it);
            break;
        } else {
            ++it;
        }
    }
    // table_schemas_.erase(
    //     std::remove_if(table_schemas_.begin(), table_schemas_.end(),
    //         [&selected_tb_name](const TableSchema& schema) {
    //             return schema.table_name_ == std::string(selected_tb_name.mb_str());
    //         }),
    //     table_schemas_.end()
    // );
}

TableSchema DatabaseTableManager::fetch_table_schema(const std::string table_name) {
    TableSchema schema;
    schema.table_name_ = table_name;

    try {
        std::string pragma_sql = "PRAGMA table_info(" + table_name + ")";

        db_->execute_sql(pragma_sql, [&schema](const std::vector<std::string>& col_names, const std::vector<std::string>& row_values) {
            ColumnDefinition col_def(row_values);

            // Debug
            assert(!col_def.name_.empty());
            //

            schema.col_defs_.push_back(col_def);
        });
    } catch (DatabaseException e) {
        std::cerr << "failed to fetch_table_schema for " << table_name << ": " << e.what();
    }
    return schema;
}

std::vector<std::string> DatabaseTableManager::get_table_names() const {
    std::vector<std::string> table_names;
    for (const auto& table_schema : table_schemas_) {
        table_names.push_back(table_schema.table_name_);
    }
    return table_names;
}