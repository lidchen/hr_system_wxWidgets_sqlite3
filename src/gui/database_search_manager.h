#ifndef DATABASE_SEARCH_MANAGER_H_
#define DATABASE_SEARCH_MANAGER_H_

#include <string>
#include "core/database/database_ops_base.h"

class DatabaseSearchManager {
public:
    DatabaseSearchManager();
    DatabaseSearchManager(const DatabaseSearchManager& other) = delete;
    DatabaseSearchManager& operator=(const DatabaseSearchManager& other) = delete;
    ~DatabaseSearchManager() = default;
    std::string build_search_sql(const std::string& key);
private:
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