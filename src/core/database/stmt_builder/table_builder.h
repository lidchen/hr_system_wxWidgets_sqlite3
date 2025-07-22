#ifndef TABLE_BUILDER_H_
#define TABLE_BUILDER_H_

#include <vector>
#include "database_stmt_builder_exception.h"
#include "table_schema.h"

// This builder is different from where builder, and its a singleton
// It works based on column_definition. The ideal case:
// wxDialog generate col_def, pass to here, create table at database

// TODO:
// Maybe I should do this to where_builder as well, for advance search

class TableBuilder {
public:
    static TableBuilder& getInsetance() {
        static TableBuilder instance;
        return instance;
    }
    std::string build_sql(const TableSchema& table_schema);
    TableBuilder(const TableBuilder&) = delete;
    TableBuilder& operator=(const TableBuilder&) = delete;
private:
    TableBuilder() = default;
    ~TableBuilder() = default;
    std::string stmt_ = "";
};

#endif // TABLE_BUILDER_H_