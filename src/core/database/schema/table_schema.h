#ifndef TABLE_SCHEMA_H_
#define TABLE_SCHEMA_H_

#include "column_definition.h"

struct TableSchema {
    std::string table_name_;
    std::vector<ColumnDefinition> col_defs_;
    std::string build_sql() const {
        std::string sql = "CREATE TABLE " + table_name_ + " (";
        for (size_t i = 0; i < col_defs_.size(); ++i) {
            sql += " " + col_defs_[i].col_def_to_string();
            if (i < col_defs_.size() - 1) sql += ",";
        }
        sql += ")";
        return sql; 
    }
    int get_row_num() {
        return col_defs_.size();
    }
};

#endif // TABLE_SCHEMA_H_