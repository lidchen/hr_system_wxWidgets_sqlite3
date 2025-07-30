#ifndef WHRER_BUILDER_H_
#define WHRER_BUILDER_H_

#include <string>
#include <vector>
#include "database_stmt_builder_exception.h"

class WhereBuilder {
private:
    std::string stmt_;
    struct Condition {
        std::string column;
        std::string value;
        bool case_sensitive = false;
        bool fuzzy_match = true;
    };
    
    std::vector<Condition> conditions_;
    std::optional<Condition> current_; // temp, until value is added
public:
    WhereBuilder(const std::string& stmt);
    WhereBuilder& column(const std::string& col_name);
    WhereBuilder& equals(const std::string& value);
    WhereBuilder& like(const std::string & value);
    WhereBuilder& case_sensitive();
    std::string build_with_and();
    std::string build_with_or();
    std::string generate_where_condition(const Condition& cond);

};

#endif // WHERE_BUILDER_H_