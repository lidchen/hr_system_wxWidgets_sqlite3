#ifndef CLI_UTIL_H_
#define CLI_UTIL_H_

#include <string>

#include "wx/wx.h"
#include "sqlite3.h"
#include "core/database/database.h"

namespace CLIUtil {
    template<typename t>
    void print(const t& value) {
        std::cout << "======================\n";
        std::cout << value << "\n";
    }
    template<typename t>
    void print(const t& value, const std::string& label) {
        std::cout << "[" + label + "]" << value << "\n";
    }
    void split();
    void print_table(const Database& db, const std::string& table_name);
    void print_query_result(const Database::QueryResult& result);
}

#endif // CLI_UTIL_H_