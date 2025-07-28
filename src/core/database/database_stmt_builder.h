#ifndef DATABASE_STMT_BUILDER_H_
#define DATABASE_STMT_BUILDER_H_

#include "sqlite3.h"
#include "string"
#include "core/database/stmt_builder/where_builder.h"

class DatabaseStmtBuilder {
private:
    std::string stmt_;
public:
    // CREATE TABLE
    DatabaseStmtBuilder() = default;
    DatabaseStmtBuilder& create_table(const std::string& table_name);
    DatabaseStmtBuilder& add_column(const std::string& column_def);
    DatabaseStmtBuilder& primary_key(const std::string& column);

    // SELECT
    DatabaseStmtBuilder& select(const std::string& select_columns);
    DatabaseStmtBuilder& from(const std::string& table_name);

    // UPDATE
    DatabaseStmtBuilder& update(const std::string& table_name);
    DatabaseStmtBuilder& set(const std::string& col_name, const std::string& new_value);

    // INSERT
    DatabaseStmtBuilder& insert_into(const std::string& table_name);
    DatabaseStmtBuilder& values(const std::vector<std::string> values);

    // DELETE
    DatabaseStmtBuilder& delete_from(const std::string& table_name);

    WhereBuilder where();

    DatabaseStmtBuilder& group_by(const std::string &grouped_columns);
    DatabaseStmtBuilder& having(const std::string& condition);
    DatabaseStmtBuilder& AND(const std::string& condition);
    DatabaseStmtBuilder& OR(const std::string& condition);
    std::string build();
};

#endif // DATABASE_STMT_BUILDER_H_