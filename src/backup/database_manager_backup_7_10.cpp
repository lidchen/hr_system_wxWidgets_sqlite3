#include "database_manager.h"
#include "database_exception.h"
#include <iostream>

Database::Database(const std::string& filename) {
    int rc = sqlite3_open(filename.c_str(), &db_);
    if (rc != SQLITE_OK) {
        std::string err_msg = sqlite3_errmsg(db_);
        sqlite3_close(db_);
        throw DatabaseException(err_msg);
    }
}

Database::~Database() {
    if (db_) {
        sqlite3_close(db_);
    }
}

Database::QueryResult Database::fetch_all(const std::string& sql) const {
    Database::QueryResult result;
    RowCallback callback = [&result](std::vector<std::string> col_name
                                    , std::vector<std::string> row_value) {
        result.column_names_ = col_name;
        result.row_values_.emplace_back(row_value);
    }; 
    this->execute_sql(sql, callback);
    return result;
}

void Database::execute_sql(const std::string& sql, RowCallback callback) const {
    if (!callback) {
        std::cout << "Use default callback function\n";
        callback = [](const std::vector<std::string> col_names
                    , const std::vector<std::string> row_values) {
            static bool is_first = true;
            if (is_first) {
                is_first = false;
                for (const auto& col_name : col_names) {
                    std::cout << col_name << " ";
                }
                std::cout << "\n";
            }
            for (const auto& row_value : row_values) {
                std::cout << row_value << " ";
            } 
            std::cout << "\n";
        };
    }

    auto wrapper_callback = [](void* data, int count, char** value, char** columns) -> int {
        RowCallback* cb = static_cast<RowCallback*>(data);
        std::vector<std::string> col_names;
        std::vector<std::string> row_values;
        for (int i = 0; i < count; ++i) {
            row_values.emplace_back(value[i] ? value[i] : "");
        }
        for (int i = 0; i < count; ++i) {
            col_names.emplace_back(columns[i] ? columns[i] : "");
        }
        (*cb)(col_names, row_values);
        return 0;
    };

    char* err_msg = nullptr;
    int rc = sqlite3_exec(db_, sql.c_str(), wrapper_callback, &callback, &err_msg);
    if (rc != SQLITE_OK) {
        std::string error_message = err_msg ? err_msg : "Unknown Error";
        if (err_msg) sqlite3_free(err_msg);
        throw(DatabaseException(error_message));
    }
    std::cout << "sql executed successfully\n";
}

// std::vector<std::string> Database::get_column_name(const std::string& sql) const {
//     std::vector<std::string> column_names;
//     sqlite3_stmt* stmt = nullptr;
//     // std::string query = sql.empty() ? "SELECT * FROM sqlite_master LIMIT 1;" : sql;
//     std::string query = sql.empty() ? "SELECT * FROM STUDENT LIMIT 1;" : sql;

//     if (sqlite3_prepare_v2(db_, query.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
//         int col_count = sqlite3_column_count(stmt);
//         for (int i = 0; i < col_count; ++i) {
//             const char* col_name = sqlite3_column_name(stmt, i);
//             if (col_name) {
//                 column_names.emplace_back(col_name);
//             }
//         }
//         if (sqlite3_finalize(stmt) != SQLITE_OK) {
//             std::string error_message = sqlite3_errmsg(db_);
//             throw(DatabaseException(error_message));
//         }
//         return column_names;
//     }
//     else {
//         std::string error_message = sqlite3_errmsg(db_);
//         throw(DatabaseException(error_message));
//     }
// }