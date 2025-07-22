#include <iostream>
#include "database.h"
#include "database_exception.h"
#include "wx/filename.h"
#include "fstream"
#include "sstream"

Database::Database(const wxString& db_path) 
{
    // For database manager
    int rc = sqlite3_open(db_path.c_str(), &db_);
    if (!db_ || rc != SQLITE_OK) {
        std::string err_msg = db_ ? sqlite3_errmsg(db_) : "Failed to create database handle";
        if (db_) sqlite3_close(db_);
        throw DatabaseException(err_msg);
    }
    // Extract filename from db_path (remove directory part)
    size_t pos = db_path.find_last_of(wxFileName::GetPathSeparator());
    filename_ = (pos != std::string::npos) ? db_path.substr(pos + 1) : db_path;

// THIS SHIT STOP ME FOR HALF HOUR TO DEBUG
// Because it close the database, damnit!
// sqlite3_close(db_);
}
Database::~Database() {
    if (db_) {
        sqlite3_close(db_);
    }
}

void Database::create_table_from_csv(const std::string& table_name, const std::string& filepath) {
    // Create stmt builder when needed is not effcient
    // , But since this code might just run few time
    // , for now it won't affect performance
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw DatabaseException("Cannot open csv file: " + filepath);
    }
    std::string line;
    std::stringstream ss;
    DatabaseStmtBuilder builder;
    builder.create_table(table_name);
    if (std::getline(file, line)) {
        ss.str(line);
        ss.clear();
        std::string token;
        while (std::getline(ss, token, ',')) {
            token += " TEXT";
            builder.add_column(token);
        }
    }
    std::string sql = builder.build();
    execute_sql(sql);
}

void Database::import_data_from_csv(const std::string& table_name, const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw DatabaseException("Cannot open csv file: " + filepath);
    }

    std::string line;
    size_t col_count;
    // get column numbers for insert stmt preparation
    if (std::getline(file, line)) {
        col_count = std::count(line.begin(), line.end(), ',') + 1;
    }

    // Prepare Insert Stmt
    std::string insert_sql = "INSERT INTO " + table_name + " VALUES (";
    for (size_t i = 0; i < col_count; ++i) {
        insert_sql += (i == 0 ? "?" : ",?");
    }
    insert_sql += ")";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db_, insert_sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw DatabaseException("Failed to prepare statement: " + std::string(sqlite3_errmsg(db_)));
    }

    while(std::getline(file, line)) {
        std::vector<std::string> tokens;
        tokens.reserve(col_count);
        std::stringstream ss(line);
        std::string token;

        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }
        // Bind values and execute
        for (size_t i = 0; i < col_count; ++i) {
            sqlite3_bind_text(stmt, i + 1, tokens[i].c_str(), -1, SQLITE_STATIC);
        }
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            throw DatabaseException("Failed to insert row: " + std::string(sqlite3_errmsg(db_)));
        }
        sqlite3_reset(stmt);
    }
    sqlite3_finalize(stmt);
}

bool Database::is_connected() const{
    return db_ ? true : false;
}
void Database::close() {
    if (db_) {
        sqlite3_close(db_);
    }
}
void Database::begin_transaction() {
    execute_sql("BEGIN TRANSACTION");
}
void Database::commit_transaction() {
    execute_sql("COMMIT");
}
void Database::rollback_transaction() {
    execute_sql("ROLLBACK");
}
void Database::debug_state() const {
    std::cout << "Database state check:" << std::endl;
    std::cout << "  db_ pointer: " << (void*)db_ << std::endl;
    if (db_) {
        std::cout << "  Last error: " << sqlite3_errmsg(db_) << std::endl;
        std::cout << "  Extended error: " << sqlite3_extended_errcode(db_) << std::endl;
    }
}

bool Database::QueryResult::is_empty() {
    return row_values_.empty();
}

Database::QueryResult Database::fetch_all(const std::string& sql) const {
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

void Database::execute_sql(const std::string& sql) const {
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
void Database::execute_sql(const std::string& sql, RowCallback callback) const {
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
int Database::get_col_num(const std::string& table_name) const {
    int col_num = 0;
    std::string sql = "PRAGMA table_info(" + table_name + ");";
    execute_sql(sql, [&col_num](const std::vector<std::string>& col_names, const std::vector<std::string>& row_values) {
        ++col_num;
    });
    return col_num;
}

// Get the number of rows in a table
int Database::get_row_num(const std::string& table_name) const {
    int row_num = 0;
    std::string sql = "SELECT COUNT(*) FROM " + table_name + ";";
    execute_sql(sql, [&row_num](const std::vector<std::string>& col_names, const std::vector<std::string>& row_values) {
        if (!row_values.empty()) {
            row_num = std::stoi(row_values[0]);
        }
    });
    return row_num;
}
