#include "database_table_manager.h"
#include "cli/cli_util.h"

DatabaseTableManager::DatabaseTableManager(Database* db)
    : db_(db) 
{
    scan_existing_tables();
    // Hack, set default table name to first one
    if (!table_schemas_.empty()) {
        current_table_name_ = table_schemas_.begin()->first;
    } else {
        throw DatabaseException("current table is empty");
    }
}

void DatabaseTableManager::scan_existing_tables() {
    try {
        std::string sql = "SELECT name FROM sqlite_master WHERE type = 'table' AND name NOT LIKE 'sqlite_%'";
        table_schemas_.clear();
        db_->execute_sql(sql, [this](const std::vector<std::string>& col_names, const std::vector<std::string> row_values){
            std::string table_name = row_values[0];
            TableSchema schema = fetch_table_schema(table_name);
            table_schemas_[table_name] = schema;
        });
    } catch (DatabaseException e) {
        std::cerr << "Failded to scan tables" << e.what() << "\n";
    }
}
void DatabaseTableManager::create_table(const TableSchema& schema) {
    // TODO: 
    // add a schema.is_valid() function
    // add table_exists(schema.name) function
    if (!(schema.validate_contains_pk())) {
        throw DatabaseException("Table Should Contain Primary Key");
    }
    std::string sql = schema.build_sql();
    db_->execute_sql(sql);
    table_schemas_[schema.table_name_] = schema;
}
void DatabaseTableManager::drop_table(const std::string& selected_tb_name) {
    std::string sql = "DROP TABLE '" + selected_tb_name + "'";
    db_->execute_sql(sql);

    table_schemas_.erase(selected_tb_name);
}
void DatabaseTableManager::rename_table(const std::string& old_name, const std::string& new_name) {
    std::string sql = "ALTER TABLE '" + old_name + "' RENAME TO '" + new_name + "'"; 
    db_->execute_sql(sql);

    // Update table_schemas_ map
    auto it = table_schemas_.find(old_name);
    if (it != table_schemas_.end()) {
        TableSchema schema = it->second;
        schema.table_name_ = new_name;
        table_schemas_.erase(it);
        table_schemas_[new_name] = schema;
    }

    // Update current_table_name_
    if (current_table_name_ == old_name) {
        current_table_name_ = new_name;
    }
}
void DatabaseTableManager::set_current_table_name(const std::string& table_name) {
    if (table_schemas_.find(table_name) != table_schemas_.end()) {
        current_table_name_ = table_name;
    }
    else {
        throw DatabaseException("Cant's set current_table_name");
    }
}
std::string DatabaseTableManager::get_current_table_name() const {
    if (current_table_name_.empty()) {
        throw DatabaseException("current_table_name is not set");
    }
    return current_table_name_;
}
TableSchema DatabaseTableManager::get_current_table_schema() const {
    if (current_table_name_.empty()) {
        throw DatabaseException("current_table_name is not set");
    }
    return table_schemas_.at(current_table_name_);
}
TableSchema DatabaseTableManager::fetch_table_schema(const std::string table_name) {
    TableSchema schema;
    schema.table_name_ = table_name;

    try {
        std::string pragma_sql = "PRAGMA table_info('" + table_name + "')";

        db_->execute_sql(pragma_sql, [&schema](const std::vector<std::string>& col_names, const std::vector<std::string>& row_values) {
            ColumnDefinition col_def(row_values);

            assert(!col_def.name_.empty());

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
        table_names.push_back(table_schema.first);
    }
    return table_names;
}

void DatabaseTableManager::add_column(const ColumnDefinition& col_def) {
    std::string sql = "ALTER TABLE ";
    sql += "'" + current_table_name_ + "'";
    sql += " ADD ";
    sql += col_def.col_def_to_string();
    db_->execute_sql(sql);
    auto& current_table_schema = table_schemas_.at(current_table_name_);
    current_table_schema.add_col(col_def);
}

void DatabaseTableManager::drop_column(const std::string& col_name) {
    std::string sql = "ALTER TABLE ";
    sql += current_table_name_;
    sql += " DROP COLUMN ";
    sql += col_name;
    db_->execute_sql(sql);
    auto& current_table_schema = table_schemas_.at(current_table_name_);
    for (auto it = current_table_schema.col_defs_.begin(); it != current_table_schema.col_defs_.end(); ++it) {
        if (it->name_ == col_name) {
            current_table_schema.col_defs_.erase(it);
            break;
        }
    }
}