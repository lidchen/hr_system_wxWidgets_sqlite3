#ifndef DATABASE_TABLE_MANAGER
#define DATABASE_TABLE_MANAGER

#include <string>
#include <vector>
#include "database.h"

class DatabaseTableManager {
public:
    DatabaseTableManager(Database* db); 
    DatabaseTableManager(const DatabaseTableManager& other) = delete;
    DatabaseTableManager& operator=(const DatabaseTableManager& other) = delete;
    // This should call wx_table_manager_panel
    void create_table();
    std::vector<std::string> get_table_names_() const;
private:
    std::vector<std::string> table_names_;    
    Database* db_;
};


#endif // DATABASE_TABLE_MANAGER