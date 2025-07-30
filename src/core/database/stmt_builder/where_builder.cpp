#include "where_builder.h"
WhereBuilder::WhereBuilder(const std::string& stmt)
    : stmt_(stmt) {}
WhereBuilder& WhereBuilder::column(const std::string& col_name) {
    current_ = Condition{.column = col_name};
    return *this;
}
WhereBuilder& WhereBuilder::equals(const std::string& value) {
    if (!current_) throw DatabaseStmtBuilderException("column() must be called before equals()");
    current_->value = value;
    current_->fuzzy_match = false;
    conditions_.push_back(*current_);
    current_.reset();
    return *this;
}
WhereBuilder& WhereBuilder::like(const std::string& value) {
    if (!current_) throw DatabaseStmtBuilderException("column() must be called before like()");
    current_->fuzzy_match = true;
    current_->value = value;
    conditions_.push_back(*current_);
    current_.reset();
    return *this;
}
WhereBuilder& WhereBuilder::case_sensitive() {
    if (conditions_.empty()) 
        throw DatabaseStmtBuilderException("No condition to apply ignore_case to");
    conditions_.back().case_sensitive = true;
    return *this;
}
std::string WhereBuilder::build_with_and(){
    stmt_ += "WHERE ";
    bool first = true;
    for (const auto& cond : conditions_) {
        if (!first) stmt_ += " AND ";
        first = false;

        stmt_ += generate_where_condition(cond);
    }
    return stmt_;
}

std::string WhereBuilder::build_with_or() {
    stmt_ += "WHERE ";
    bool first = true;
    for (const auto& cond : conditions_) {
        if (!first) stmt_ += " OR ";
        first = false;
        stmt_ += generate_where_condition(cond);
    }
    return stmt_;
}
std::string WhereBuilder::generate_where_condition(const Condition& cond) {
    std::string col = cond.column;
    std::string op;
    std::string value = cond.value;
    if (cond.case_sensitive) {
        op = "GLOB";
        if (cond.fuzzy_match) value += "*";
    } else {
        op = "LIKE";
        if (cond.fuzzy_match) value += "%";
    }
    return col + " " + op + " '" + value + "'";  
}