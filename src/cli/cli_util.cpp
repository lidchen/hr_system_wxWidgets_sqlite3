#include <iostream>

#include "cli_util.h"

void CLIUtil::print(const std::string& info) {
    std::cout << "============String=============\n";
    std::cout << info << "\n";
}

void CLIUtil::print(const wxString& info) {
    std::cout << "===========wxString============\n";
    std::cout << info << "\n";
}

void CLIUtil::print_table(const Database& db, const std::string& table_name) {
    auto result = db.fetch_all("SELECT * FROM " + table_name);
    print_query_result(result);
}

void CLIUtil::print_query_result(const Database::QueryResult& result) {
    for (auto i : result.column_names_) {
        std::cout << i << " ";
    }
    std::cout << "\n";
    for (auto i : result.row_values_) {
        for (auto j : i) {
            std::cout << j << " ";
        }
        std::cout << "\n";
    }
}