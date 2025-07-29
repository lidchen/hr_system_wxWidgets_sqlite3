#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "database_stmt_builder.h"

#include "sqlite3.h"

DatabaseStmtBuilder& DatabaseStmtBuilder
    ::create_table(const std::string& table_name) {
    stmt_ = "CREATE TABLE " + table_name + "(";
    return *this;
}

DatabaseStmtBuilder& DatabaseStmtBuilder
    ::add_column(const std::string& column_def) {
    if (stmt_.back() != '(') {
        stmt_ += ", ";
    }
    stmt_ += column_def;
    return *this;
}
DatabaseStmtBuilder& DatabaseStmtBuilder
    ::primary_key(const std::string& column) {
    stmt_ += ", PRIMARY KEY (" + column + ")";
    return *this;
}
DatabaseStmtBuilder& DatabaseStmtBuilder
    ::select(const std::string& select_columns) {
    stmt_ = "SELECT " + select_columns + " ";
    return *this;
}
DatabaseStmtBuilder& DatabaseStmtBuilder
    ::from(const std::string& table_name) {
    stmt_ += "FROM " + table_name + " ";
    return *this;
}

// UPDATE
DatabaseStmtBuilder& DatabaseStmtBuilder
    ::update(const std::string& table_name) {
    stmt_ = "UPDATE " + table_name + " ";
    return *this;
}

DatabaseStmtBuilder& DatabaseStmtBuilder
    ::set(const std::string& col_name, const std::string& new_value) {
    stmt_ += "SET " + col_name + " = '" + new_value + "' ";
    return *this;
}

// INSERT
DatabaseStmtBuilder& DatabaseStmtBuilder
    ::insert_into(const std::string& table_name) {
    stmt_ = "INSERT INTO ";
    stmt_ += (table_name + " ");
    return *this;
}
DatabaseStmtBuilder& DatabaseStmtBuilder
    ::values(std::vector<std::string> values) {
    std::ostringstream oss;
    oss << "VALUES (";
    for (size_t i = 0; i < values.size(); ++i) {
        oss << "'" << values[i] << "'";
        if (i != values.size() - 1) oss << ",";
    }
    oss << ")";
    stmt_ += oss.str();
    return *this;
}

// DELETE
DatabaseStmtBuilder& DatabaseStmtBuilder
    ::delete_from(const std::string& table_name) {
    stmt_ = "DELETE FROM " + table_name + " ";
    return *this;
}

// Not sure about this implematation
WhereBuilder DatabaseStmtBuilder::where() {
    return WhereBuilder(stmt_);
}
DatabaseStmtBuilder& DatabaseStmtBuilder
    ::AND(const std::string& condition) {
    stmt_ += "AND (" + condition + ") "; 
    return *this;
}
DatabaseStmtBuilder& DatabaseStmtBuilder
    ::OR(const std::string& condition) {
    stmt_ += "OR (" + condition + ") ";
    return *this;
}
std::string DatabaseStmtBuilder
    ::build() {
    std::string result = stmt_;
    if (result.find("CREATE TABLE") != std::string::npos) {
        result += ")"; 
    }
    return result;
}
