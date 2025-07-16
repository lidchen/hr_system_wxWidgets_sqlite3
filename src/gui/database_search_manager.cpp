#include <string>
#include "database_search_manager.h"
#include "core/database/database_stmt_builder.h"

DatabaseSearchManager::DatabaseSearchManager() {
    search_options.fuzzy_search = false;
}
std::string DatabaseSearchManager::build_search_sql(const std::string& key) {
    std::string value = key;
    size_t pos = 0;
    while ((pos = value.find("'", pos)) != std::string::npos) {
        value.replace(pos, 1, "''");
        pos += 2;
    }
    DatabaseStmtBuilder builder;
    auto stmt = builder.select("*")
        .from("employee")
        .where()
        .column("FIRST_NAME").like(value).ignore_case()
        .build_with_or();
    return stmt;
}