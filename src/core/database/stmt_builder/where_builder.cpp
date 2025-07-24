#include "where_builder.h"
WhereBuilder::WhereBuilder(const std::string& stmt)
    : stmt_(stmt) {}
WhereBuilder& WhereBuilder::column(const std::string& col_name) {
    current_ = Condition{.column = col_name};
    return *this;
}
WhereBuilder& WhereBuilder::equals(const std::string& value) {
    if (!current_) throw DatabaseStmtBuilderException("column() must be called before equals()");
    current_->op = "=";
    current_->value = value;
    conditions_.push_back(*current_);
    current_.reset();
    return *this;
}
WhereBuilder& WhereBuilder::like(const std::string& value) {
    if (!current_) throw DatabaseStmtBuilderException("column() must be called before like()");
    current_->op = "LIKE";
    current_->value = value + "%"; // prefix match
    conditions_.push_back(*current_);
    current_.reset();
    return *this;
}
WhereBuilder& WhereBuilder::ignore_case() {
    if (conditions_.empty() && !current_)
        throw DatabaseStmtBuilderException("No condition to apply ignore_case to");

    if (current_) {
        current_->ignore_case = true;
    } else {
        conditions_.back().ignore_case = true;
    }
    return *this;
}
std::string WhereBuilder::build_with_and(){
    stmt_ += "WHERE ";
    bool first = true;
    for (const auto& cond : conditions_) {
        if (!first) stmt_ += " AND ";
        first = false;

        std::string col = cond.ignore_case ? "LOWER(" + cond.column + ")" : cond.column;
        std::string val = cond.ignore_case ? "LOWER('" + cond.value + "')" : "'" + cond.value + "'";
        stmt_ += col + " " + cond.op + " " + val;
    }
    return stmt_;
}
std::string WhereBuilder::build_with_or() {
    stmt_ += "WHERE ";
    bool first = true;
    for (const auto& cond : conditions_) {
        if (!first) stmt_ += " OR ";
        first = false;

        std::string col = cond.ignore_case ? "LOWER(" + cond.column + ")" : cond.column;
        std::string val = cond.ignore_case ? "LOWER('" + cond.value + "')" : "'" + cond.value + "'";
        stmt_ += col + " " + cond.op + " " + val;
    }
    return stmt_;
}