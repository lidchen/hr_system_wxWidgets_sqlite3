#ifndef DATABASE_GRID_MANAGER_H_
#define DATABASE_GRID_MANAGER_H_

#include "wx/wx.h"
#include "wx/grid.h"
#include "core/database/database.h"
#include "core/database/database_manager.h"
#include "core/database/database_table_manager.h"
#include "core/database/schema/table_schema.h"

class DatabaseGridManager {
public:
    DatabaseGridManager(wxGrid* grid);
    void construct_grid(const TableSchema &schema);
    void construct_by_result(const Database::QueryResult &q_result);
    void clear_grid();
private:
    std::unique_ptr<DatabaseTableManager> tb_manager_;
    wxGrid* wx_grid_ = nullptr; 
    DatabaseManager& db_manager_ = DatabaseManager::getInstance();
};

#endif // DATABASE_GRID_H_
