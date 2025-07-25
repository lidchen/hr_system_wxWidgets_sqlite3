#include "table_schema.h"
#include "assert.h"
#include "cli/cli_util.h"

std::vector<std::string> TableSchema::get_col_names() const {
    std::vector<std::string> col_names;
    for (const auto& col_def : col_defs_) {
        col_names.push_back(col_def.name_);
    }
    return col_names;
}

std::string TableSchema::build_sql() const {
    std::string sql = "CREATE TABLE " + table_name_ + " (";
    for (size_t i = 0; i < col_defs_.size(); ++i) {
        sql += " " + col_defs_[i].col_def_to_string();
        if (i < col_defs_.size() - 1) sql += ",";
    }
    sql += ")";
    return sql; 
}

std::string TableSchema::get_pk_name() const {
    for (auto col_def : col_defs_) {
        if (col_def.is_pk()) {
            return col_def.name_;
        }
    }
    assert(false && "No primary key found in TableSchema");
    return "CANT FIND PRIMARY KEY";
}
int TableSchema::get_pk_index() const {
    for (int i = 0; i < col_defs_.size(); ++i) {
        if (col_defs_[i].is_pk()) {
            return i;
        }
    }
    assert(false && "No primary key found in TableSchema");
    return -1;
}

int TableSchema::get_row_num() {
    return col_defs_.size();
}

void TableSchema::add_col(const ColumnDefinition& col_def) {
    col_defs_.push_back(col_def);
}

// bool TableSchema::validate(int col_index, std::string new_value) {
//     const auto& col_def = col_defs_[col_index];
//     assert(!col_def.name_.empty() && !new_value.empty());

//     switch(col_def.type_) {

//     }
// }