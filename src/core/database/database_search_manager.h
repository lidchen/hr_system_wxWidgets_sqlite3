#ifndef DATABASE_SEARCH_MANAGER_H_
#define DATABASE_SEARCH_MANAGER_H_

#include <string>
#include "core/database/database_ops_base.h"

// For advance search
// Each col should have default SearchType when create table
// At advance search user can set search type for each col
enum class SearchType {
    EXACT_MATCH,    // For IDs, booleans
    PARTIAL_MATCH,  // For text fields
    RANGE,          // For numbers, dates
    LIST_MATCH      // For dropdown/enum values
};

class DatabaseSearchManager {
public:
    DatabaseSearchManager();
    DatabaseSearchManager(const DatabaseSearchManager& other) = delete;
    DatabaseSearchManager& operator=(const DatabaseSearchManager& other) = delete;
    ~DatabaseSearchManager() = default;
    std::string build_search_sql(const std::string& key);
private:

    // TODO
    // This one is for simple search's column range
    // Show use a dropdown checkbox to select
    // After implementation of dynamic table make this auto-detected
    struct SearchOptions {
        bool fuzzy_search;
        bool search_first_name;
        bool search_last_name;
        bool search_email;
        bool search_gender;
        bool search_phone;
        bool search_birth;
        bool search_title;
        // ... TODO ...
    }search_options;
};

#endif // DATABASE_SEARCH_MANAGER_H_