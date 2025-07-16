#ifndef DATABASE_FILE_HANDLER_H_
#define DATABASE_FILE_HANDLER_H_

#include <fstream>
#include <vector>
#include "sqlite3.h"
#include "database_ops_base.h"

class DatabaseFileHandler : DatabaseOpsBase{
public:
    DatabaseFileHandler(sqlite3* db) : DatabaseOpsBase(db) {}
    void import_data_from_csv(const std::string& table_name, const std::string& filepath);
    void create_table_from_csv(const std::string& table_name, const std::string& filepath);
};

#endif // DATABASE_FILE_HANDLER_H_