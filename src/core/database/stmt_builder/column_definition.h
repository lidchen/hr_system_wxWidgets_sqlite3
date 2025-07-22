#ifndef COLUMN_DEFINITION_H_
#define COLUMN_DEFINITION_H_

#include <string>
#include <vector>

enum class ColumnType {
    TEXT,
    INTEGER,
    FLOAT,
    DATE,
    BOOLEAN,
    // dont know if I could do this
    // ENUM_LIKE
};

enum class ColumnConstraint {
    NONE            = 0,
    NOT_NULL        = 1 << 0,
    PRIMARY_KEY     = 1 << 1,
    // UNIQUE          = 1 << 2,
    // AUTO_INCREMENT  = 1 << 3
};

inline ColumnConstraint operator|(ColumnConstraint a, ColumnConstraint b) {
    return static_cast<ColumnConstraint>(static_cast<int>(a) | static_cast<int>(b));
}
inline bool has_constraint(ColumnConstraint constraints, ColumnConstraint check) {
    return static_cast<int>(static_cast<int>(constraints) & static_cast<int>(check)) != 0;
}
struct ColumnDefinition {
    std::string name_;
    ColumnType type_;
    ColumnConstraint constraints_ = ColumnConstraint::NONE;
    std::string default_value_;
    ColumnDefinition() = default;
    ColumnDefinition(const std::vector<std::string>& row_values) {
        generate_from_pragma_sql(row_values);
    } 

// TODO:
// std::vector<std::string> allowed_values;

    // TODO:
    // we fucked, the pragma_sql dont give information of AUTO_INCREMENT and UNIQUE
    // I just remove this two in ColumnConstraint
    // Maybe fix this later (T^T)

    // get row_values in callback function using pragma_sql
    void generate_from_pragma_sql(const std::vector<std::string>& row_values) {
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

    std::string col_def_to_string() const {
        std::string col_def_sql = name_ + " " + type_to_string(type_);
        if (has_constraint(constraints_, ColumnConstraint::PRIMARY_KEY)) {
            col_def_sql += " PRIMARY KEY";
        }
        if (has_constraint(constraints_, ColumnConstraint::NOT_NULL)) {
            col_def_sql += " NOT NULL";
        }
        // if (has_constraint(constraints_, ColumnConstraint::UNIQUE)) {
        //     col_def_sql += " UNIQUE";
        // }
        // if (has_constraint(constraints_, ColumnConstraint::AUTO_INCREMENT)) {
        //     col_def_sql += " AUTOINCREMENT";
        // }
        if (!default_value_.empty()) {
            col_def_sql += " DEFAULT '" + default_value_ + "'";
        }
        return col_def_sql;
    }

private:
    std::string type_to_string(ColumnType type) const {
        switch (type)
        {
            case ColumnType::TEXT: return "TEXT";
            case ColumnType::INTEGER: return "INTEGER";
            case ColumnType::FLOAT: return "REAL";
            case ColumnType::DATE: return "DATE";
            case ColumnType::BOOLEAN: return "BOOLEAN";
            default: return "TEXT";
        }
    }
    ColumnType string_to_type(std::string type_str) const {
        if (type_str.find("TEXT") != std::string::npos) {
            return ColumnType::TEXT;
        }
        if (type_str.find("INTEGER") != std::string::npos) {
            return ColumnType::INTEGER;
        }
        if (type_str.find("REAL") != std::string::npos) {
            return ColumnType::FLOAT;
        }
        if (type_str.find("DATE") != std::string::npos) {
            return ColumnType::DATE;
        }
        if (type_str.find("BOOLEAN") != std::string::npos) {
            return ColumnType::BOOLEAN;
        }
        return ColumnType::TEXT;
    }
};

#endif // COLUMN_DEFINITION_H_