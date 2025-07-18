#ifndef DATABASE_GRID_MANAGER_H_
#define DATABASE_GRID_MANAGER_H_

#include "wx/wx.h"
#include "wx/grid.h"
#include "core/database/database.h"
#include "core/database/database_ops_base.h"
#include "core/database/database_manager.h"

class DatabaseGridManager {
public:
    DatabaseGridManager(wxGrid* grid);
    void construct_by_result(const DatabaseOpsBase::QueryResult& q_result);
    void clear_grid();
private:
    wxGrid* wx_grid_ = nullptr; 
    DatabaseManager& db_manager_ = DatabaseManager::getInstance();
};

#endif // DATABASE_GRID_H_
