#ifndef DATABASE_TABLE_MANAGER_H_
#define DATABASE_TABLE_MANAGER_H_

#include <string>
#include <vector>
#include "database_search_manager.h" // For enum class SearchType

enum class ColType {
    TEXT,
    INTEGER,
    REAL,
    DATA,
    BOOLEAN
};

class DatabaseTableManager{
public:
    DatabaseTableManager();

};

#endif // DATABASE_TABLE_MANAGER_H_
