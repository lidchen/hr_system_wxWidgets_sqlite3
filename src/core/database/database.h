#ifndef DATABASE_H_
#define DATABASE_H_

#include <sqlite3.h>
#include <string>
#include <functional>

#include "database_exception.h"
#include "database_ops_base.h"
#include "database_stmt_builder.h"
#include "database_file_handler.h"
#include "transaction_manager.h"

#include "wx/wx.h"

class Database : public DatabaseOpsBase {
public:
    std::string filename_;
    Database (const wxString& db_filename);
    Database (const Database& other) = delete;
    Database& operator= (const Database& other) = delete;
    ~Database ();
    bool is_opened();
    void close();
    void import_from_csv(const std::string &filepath);
    void begin_transaction();
    void commit_transaction();
    void rollback_transaction();
    void debug_state() const;

    std::unique_ptr<DatabaseFileHandler> file_handler_;
private:
    // Rename this to distinguish base class's db_ and this db
    std::unique_ptr<TransactionManager> transaction_manager_;
};

#endif // DATABASE_H_