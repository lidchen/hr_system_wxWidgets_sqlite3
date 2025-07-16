#ifndef DATABASE_MANAGER_H_
#define DATABASE_MANAGER_H_

#include <sqlite3.h>
#include <string>
#include <functional>

#include "database_exception.h"
#include "database_ops_base.h"


class Database : DatabaseOpsBase {
    using RowCallback = std::function<void (const std::vector<std::string>& column_names,
                                            const std::vector<std::string>& row_values)>;
public:
    explicit Database (const std::string& filename);
    Database (const Database& other) = delete;
    Database& operator= (const Database& other) = delete;
    ~Database ();

    class QueryResult {
    public:
        std::vector<std::string> column_names_;
        std::vector<std::vector<std::string>> row_values_;

        QueryResult() = default;
        QueryResult(const QueryResult&) = delete;
        QueryResult& operator=(const QueryResult&) = delete;
        QueryResult(QueryResult&&) = default;
        QueryResult& operator=(QueryResult&&) = default;
        ~QueryResult() = default;
    };
    QueryResult fetch_all(const std::string &sql) const;

    void execute_sql(const std::string& sql, RowCallback callback) const;
    // std::vector<std::string> get_column_name(const std::string& sql) const;
protected:
    sqlite3* db_ = nullptr;
};

#endif