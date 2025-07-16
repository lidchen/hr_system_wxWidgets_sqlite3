#ifndef CLI_UTIL_H_
#define CLI_UTIL_H_

#include <string>

#include "sqlite3.h"
#include "core/database/database_manager.h"

namespace CLIUtil {
    void print_table(const Database& db, const std::string& table_name);
    void print_query_result(const DatabaseOpsBase::QueryResult& result);
}

#endif // CLI_UTIL_H_