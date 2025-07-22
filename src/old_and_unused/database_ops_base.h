#ifndef DATABASE_OPS_BASE_H_
#define DATABASE_OPS_BASE_H_

#include <string>
#include <functional>
#include <vector>

#include "sqlite3.h"
#include "database_exception.h"

class DatabaseOpsBase {
public:
    using RowCallback = std::function<void (const std::vector<std::string>& column_names
                                        , const std::vector<std::string>& row_values)>;
    class QueryResult {
    public:
        std::vector<std::string> column_names_;
        std::vector<std::vector<std::string>> row_values_;
        int col_num_ = 0;
        int row_num_ = 0;

        QueryResult() = default;
        QueryResult(const QueryResult&) = delete;
        QueryResult& operator=(const QueryResult&) = delete;
        QueryResult(QueryResult&&) = default;
        QueryResult& operator=(QueryResult&&) = default;
        ~QueryResult() = default;
        bool is_empty();
    };
    virtual ~DatabaseOpsBase() = default;
    QueryResult fetch_all(const std::string& sql) const;

    void execute_sql(const std::string& sql, RowCallback callback) const;
    void execute_sql(const std::string& sql) const;

    int get_col_num(const std::string& table_name) const;
    int get_row_num(const std::string& table_name) const;

    bool is_connected() const;
    void debug_state() const;
protected:
    DatabaseOpsBase(sqlite3* db) : db_(db) {}
    sqlite3* db_;
};

#endif // DATABASE_OPS_BASE_H_