#include "column_definition.h"
#include "iostream"

ColumnDefinition::ColumnDefinition(const std::string& name, const std::string& type, const std::string& constraints) {
    name_ = name;
    type_ = string_to_type(type);
    constraints_ = string_to_constraints(constraints);
}
ColumnDefinition::ColumnDefinition(const std::vector<std::string>& row_values) {
    generate_from_pragma_sql(row_values);
} 
// void set_type_from_string(const std::string& str) {
//     type_ = string_to_type(str);
// }
// void set_constraints_from_string(const std::string& str) {
//     constraints_ = string_to_constraints(str);
// }

// TODO:
// we fucked, the pragma_sql dont give information of AUTO_INCREMENT and UNIQUE
// I just remove this two in ColumnConstraint
// Maybe fix this later (T^T)

// get row_values in callback function using pragma_sql
void ColumnDefinition::generate_from_pragma_sql(const std::vector<std::string>& row_values) {
    name_ = row_values[1];
    std::string type_str = row_values[2];
    std::transform(type_str.begin(), type_str.end(), type_str.begin(), ::toupper);
    type_ = string_to_type(type_str);
    constraints_ = ColumnConstraint::NONE;
    if (row_values[3] == "1") {
        constraints_ = constraints_ | ColumnConstraint::NOT_NULL;
    }
    if (row_values[5] == "1") {
        constraints_ = constraints_ | ColumnConstraint::PRIMARY_KEY;
    }

    if (!row_values[4].empty() && row_values[4] != "NULL") {
        default_value_ = row_values[4];
    }
}

std::string ColumnDefinition::col_def_to_string() const {
    std::string col_def_sql = name_ + " " + type_to_string();
    col_def_sql += " ";
    col_def_sql += constraints_to_string();
    return col_def_sql;
}

std::string ColumnDefinition::constraints_to_string() const {
    std::string sql = "";
    if (has_constraint(constraints_, ColumnConstraint::PRIMARY_KEY)) {
        sql += " PRIMARY KEY";
    }
    if (has_constraint(constraints_, ColumnConstraint::NOT_NULL)) {
        sql += " NOT NULL";
    }
    // if (has_constraint(constraints_, ColumnConstraint::UNIQUE)) {
    //     col_def_sql += " UNIQUE";
    // }
    // if (has_constraint(constraints_, ColumnConstraint::AUTO_INCREMENT)) {
    //     col_def_sql += " AUTOINCREMENT";
    // }
    if (!default_value_.empty()) {
        sql += " DEFAULT '" + default_value_ + "'";
    }
    return sql;
}

ColumnConstraint ColumnDefinition::string_to_constraints(const std::string& constraint_str) {
    ColumnConstraint constraints = ColumnConstraint::NONE;
    std::string str = constraint_str;
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);

    if (str.find("PRIMARY KEY") != std::string::npos) {
        constraints = constraints | ColumnConstraint::PRIMARY_KEY;
    }
    if (str.find("NOT NULL") != std::string::npos) {
        constraints = constraints | ColumnConstraint::NOT_NULL;
    }
    return constraints;
}

std::string ColumnDefinition::type_to_string() const {
    switch (type_)
    {
        case ColumnType::TEXT: return "TEXT";
        case ColumnType::INTEGER: return "INTEGER";
        case ColumnType::FLOAT: return "REAL";
        case ColumnType::BOOLEAN: return "BOOLEAN";
        case ColumnType::DATE: return "DATE";
        default: return "TEXT";
    }
}

ColumnType ColumnDefinition::string_to_type(std::string type_str) const {
    if (type_str == "TEXT") { return ColumnType::TEXT; }
    if (type_str == "INTEGER") { return ColumnType::INTEGER; }
    if (type_str == "REAL") { return ColumnType::FLOAT; }
    if (type_str == "BOOLEAN") { return ColumnType::BOOLEAN; }
    if (type_str == "DATE") { return ColumnType::DATE; }
    // Test
    std::cerr << "Uncatched type at ColumnDefinition::string_to_type\n";
    return ColumnType::TEXT;
}

bool ColumnDefinition::is_pk() const{
    return has_constraint(constraints_, ColumnConstraint::PRIMARY_KEY);
}

bool ColumnDefinition::is_empty() const {
    return name_.empty();
}

// bool ColumnDefinition::validate(const std::string& value) {

// }
// bool ColumnDefinition::is_integer(const std::string& str) {

// }
// bool ColumnDefinition::is_float(const std::string& str) {

// }
// bool ColumnDefinition::is_boolean(const std::string& str) {

// }
// bool ColumnDefinition::is_date(const std::string& str) {

// }

