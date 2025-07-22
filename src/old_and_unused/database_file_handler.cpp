#include "database_file_handler.h"
#include "database_exception.h"
#include "database_stmt_builder.h"
#include "sstream"

void DatabaseFileHandler::create_table_from_csv(const std::string& table_name, const std::string& filepath) {
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

void DatabaseFileHandler::import_data_from_csv(const std::string& table_name, const std::string& filepath) {
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