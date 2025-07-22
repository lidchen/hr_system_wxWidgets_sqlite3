#ifndef DATABASE_H_
#define DATABASE_H_

#include <string>
#include <functional>

#include "wx/wx.h"
#include "sqlite3.h"

#include "database_exception.h"
#include "database_stmt_builder.h"

class Database {
public:

    std::string filename_;
    Database (const wxString& db_filename);
    Database (const Database& other) = delete;
    Database& operator= (const Database& other) = delete;
    ~Database ();

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

    QueryResult fetch_all(const std::string& sql) const;

    void import_data_from_csv(const std::string& table_name, const std::string& filepath);
    void create_table_from_csv(const std::string& table_name, const std::string& filepath);

    bool is_connected() const;
    void debug_state() const;
    void close();
    void begin_transaction();
    void commit_transaction();
    void rollback_transaction();
    void execute_sql(const std::string& sql, RowCallback callback) const;
    void execute_sql(const std::string& sql) const;
    int get_col_num(const std::string& table_name) const;
    int get_row_num(const std::string& table_name) const;
private:
    sqlite3* db_;
};

#endif // DATABASE_H_