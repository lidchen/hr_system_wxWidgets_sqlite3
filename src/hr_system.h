#ifndef HR_SYSTEM_H_
#define HR_SYSTEM_H_

#include "wx/wx.h"
#include "core/database/database.h"
#include "core/database/database_manager.h"
#include "core/database/database_exception.h"
#include "core/database/database_stmt_builder.h"
#include "gui/wx_database_grid_panel.h"

#include "gui/dialog/wx_database_manager_dialog.h"
#include "gui/dialog/wx_table_manager_dialog.h"
#include "gui/dialog/wx_table_schema_dialog.h"

class MainApp : public wxApp {
public:
    virtual bool OnInit();
};

#endif // HR_SYSTEM_H_