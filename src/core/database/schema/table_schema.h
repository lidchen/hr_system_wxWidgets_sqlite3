#ifndef TABLE_SCHEMA_H_
#define TABLE_SCHEMA_H_

#include <vector>
#include <string>
#include "column_definition.h"

class TableSchema {
public:
    TableSchema() = default;
    TableSchema(std::string table_name, std::vector<ColumnDefinition> col_defs) 
        : table_name_(table_name), col_defs_(col_defs) {}
    std::string table_name_;
    std::vector<ColumnDefinition> col_defs_;

    std::vector<std::string> get_col_names() const;
    std::string build_sql() const;
    std::string get_pk_name() const;
    int get_pk_index() const;
    int get_row_num() const;
    void add_col(const ColumnDefinition& col_def);
    bool validate_contains_pk() const;
    bool is_empty() const;
    // bool validate(int col, std::string new_value);
private:
    bool is_integer(const std::string& str);
    bool is_float(const std::string& str);
    bool is_bool(const std::string& str);
};

#endif // TABLE_SCHEMA_H_