#ifndef COLUMN_DEFINITION_H_
#define COLUMN_DEFINITION_H_

#include <string>
#include <vector>
#include "wx/wx.h"

enum class ColumnType
{
    TEXT,
    INTEGER,
    FLOAT,
    BOOLEAN,
    DATE
    // TODO
    // YEAR,
    // MONEY
    // ENUM_LIKE
};

enum class ColumnConstraint {
    NONE            = 0,
    NOT_NULL        = 1 << 0,
    PRIMARY_KEY     = 1 << 1
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
    ColumnDefinition(const std::string& name, const std::string& type, const std::string& constraints);
    ColumnDefinition(const std::vector<std::string>& row_values); 

    // void set_type_from_string(const std::string& str) ;
    // void set_constraints_from_string(const std::string& str) ;

    // TODO:
    // we fucked, the pragma_sql dont give information of AUTO_INCREMENT and UNIQUE
    // I just remove this two in ColumnConstraint
    // Maybe fix this later (T^T)

    static wxArrayString get_col_types() {
        wxArrayString arr;
        arr.Add("TEXT");
        arr.Add("INTEGER");
        arr.Add("FLOAT");
        arr.Add("BOOLEAN");
        arr.Add("DATE");
        return arr;
    }
    static wxArrayString get_col_constraints() {
        wxArrayString arr;
        arr.Add("NONE");
        arr.Add("PRIMARY KEY");
        arr.Add("NOT NULL");
        return arr;
    }

    // get row_values in callback function using pragma_sql
    void generate_from_pragma_sql(const std::vector<std::string>& row_values);
    std::string col_def_to_string() const;
    std::string constraints_to_string() const;
    ColumnConstraint string_to_constraints(const std::string& constraint_str);
    std::string type_to_string() const;
    ColumnType string_to_type(std::string type_str) const;
    bool is_pk() const;
    bool is_empty() const;

    // bool validate(const std::string& value);
private:
};

#endif // COLUMN_DEFINITION_H_