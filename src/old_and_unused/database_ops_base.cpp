#include <iostream>

#include "database_ops_base.h"
#include "cassert"

bool DatabaseOpsBase::QueryResult::is_empty() {
    return row_values_.empty();
}
DatabaseOpsBase::QueryResult DatabaseOpsBase::fetch_all(const std::string& sql) const {
// DEBUG
// debug_state();
    QueryResult result;
    RowCallback callback = [&result](const std::vector<std::string>& col_name,
                                     const std::vector<std::string>& row_value) {
        if (result.column_names_.empty()) {
            result.column_names_ = col_name;
            result.col_num_ = col_name.size();
        }
        result.row_values_.push_back(row_value);
    };
    this->execute_sql(sql, callback);
    result.row_num_ = result.row_values_.size();
    // assert
    // assert(result.col_num_ != 0 && result.row_num_ != 0);
    return result;
}
void DatabaseOpsBase::execute_sql(const std::string& sql) const {
// debug_state();
    char* err_msg = nullptr;
    int rc = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        std::string error_message = err_msg ? err_msg : "Unknown Error";
        sqlite3_free(err_msg);
        throw(DatabaseException(error_message));
    }
}

// TODO
// Split this big fuction into small ones
void DatabaseOpsBase::execute_sql(const std::string& sql, RowCallback callback) const {
    if (!callback) {
        std::cout << "Use default callback function\n";
        callback = [](const std::vector<std::string> col_names
                    , const std::vector<std::string> row_values) {
            // Print column names for each row (no static variable)
            for (const auto& col_name : col_names) {
                std::cout << col_name << " ";
            }
            std::cout << "\n";
            for (const auto& row_value : row_values) {
                std::cout << row_value << " ";
            } 
            std::cout << "\n";
        };
    }

    struct CallbackData {
        RowCallback* cb;
        std::vector<std::string> col_names;
        bool col_names_initialized = false;
    };

    auto wrapper_callback = [](void* data, int count, char** value, char** columns) -> int {
        CallbackData* cb_data = static_cast<CallbackData*>(data);

        if (!cb_data->col_names_initialized) {
            cb_data->col_names.clear();
            for (int i = 0; i < count; ++i) {
                cb_data->col_names.emplace_back(columns[i] ? columns[i] : "");
            }
            cb_data->col_names_initialized = true;
        }

        std::vector<std::string> row_values;
        for (int i = 0; i < count; ++i) {
            row_values.emplace_back(value[i] ? value[i] : "");
        }
        // Call the callback with column names and row values
        if (cb_data->cb) {
            (*(cb_data->cb))(cb_data->col_names, row_values);
        }
        return 0;
    };

    CallbackData cb_data{&callback, {}, false};

    char* err_msg = nullptr;
    int rc = sqlite3_exec(db_, sql.c_str(), wrapper_callback, &cb_data, &err_msg);
    if (rc != SQLITE_OK) {
        std::string error_message = err_msg ? err_msg : "Unknown Error";
        if (!err_msg) {
            std::cerr << "SQL: " << sql << "\nError: " << error_message << "\n";
            std::cerr << "SQLite Error Code: " << rc << std::endl;
            std::cerr << "SQLite Error Name: " << sqlite3_errstr(rc) << std::endl;
        }
        if (err_msg) sqlite3_free(err_msg);
        throw(DatabaseException(error_message));
    }
}

// Get the number of columns in a table
int DatabaseOpsBase::get_col_num(const std::string& table_name) const {
    int col_num = 0;
    std::string sql = "PRAGMA table_info(" + table_name + ");";
    execute_sql(sql, [&col_num](const std::vector<std::string>& col_names, const std::vector<std::string>& row_values) {
        ++col_num;
    });
    return col_num;
}

// Get the number of rows in a table
int DatabaseOpsBase::get_row_num(const std::string& table_name) const {
    int row_num = 0;
    std::string sql = "SELECT COUNT(*) FROM " + table_name + ";";
    execute_sql(sql, [&row_num](const std::vector<std::string>& col_names, const std::vector<std::string>& row_values) {
        if (!row_values.empty()) {
            row_num = std::stoi(row_values[0]);
        }
    });
    return row_num;
}

bool DatabaseOpsBase::is_connected() const {
    return db_ != nullptr;
}
void DatabaseOpsBase::debug_state() const {
    std::cout << "Database state check:" << std::endl;
    std::cout << "  db_ pointer: " << (void*)db_ << std::endl;
    if (db_) {
        std::cout << "  Last error: " << sqlite3_errmsg(db_) << std::endl;
        std::cout << "  Extended error: " << sqlite3_extended_errcode(db_) << std::endl;
    }
}