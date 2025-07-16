#ifndef WHRER_BUILDER_H_
#define WHRER_BUILDER_H_

#include <string>
#include <vector>
#include "database_stmt_builder_exception.h"

class WhereBuilder {
public:
    WhereBuilder(const std::string& stmt);
    WhereBuilder& column(const std::string& col_name);
    WhereBuilder& equals(const std::string& value);
    WhereBuilder& like(const std::string & value);
    WhereBuilder& ignore_case();
    std::string build_with_and();
    std::string build_with_or();
private:
    std::string stmt_;
    struct Condition {
        std::string column;
        std::string op;
        std::string value;
        bool ignore_case = false;
    };
    
    std::vector<Condition> conditions_;
    std::optional<Condition> current_; // temp, until value is added
};

#endif // WHERE_BUILDER_H_