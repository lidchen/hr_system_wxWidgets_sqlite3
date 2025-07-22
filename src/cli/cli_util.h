#ifndef CLI_UTIL_H_
#define CLI_UTIL_H_

#include <string>

#include "wx/wx.h"
#include "sqlite3.h"
#include "core/database/database.h"

namespace CLIUtil {
    void print(const std::string& info);
    void print(const wxString& info);
    void print_table(const Database& db, const std::string& table_name);
    void print_query_result(const Database::QueryResult& result);
}

#endif // CLI_UTIL_H_