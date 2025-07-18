#include "wx/wx.h"
#include "core/database/database.h"
#include "core/database/database_manager.h"
#include "core/database/database_exception.h"
#include "core/database/database_stmt_builder.h"
#include "gui/wx_database_grid_panel.h"
#include "gui/dialog/wx_database_manager_dialog.h"

class MainApp : public wxApp {
public:
    virtual bool OnInit() {
    std::string project_path = "/Users/lid/Library/CloudStorage/OneDrive-Personal/Develop/work/hm_system/";
        try{
            wxFrame* main_frame = new wxFrame(nullptr, wxID_ANY, "test", wxDefaultPosition, wxSize(700, 500));
            main_frame->Show(true);
// TEST
// wxCreateDatabaseDialog* test_dialog = new wxCreateDatabaseDialog(main_frame);
auto* test = new wxDatabaseManagerDialog(main_frame);
test->ShowModal();
        } catch(DatabaseException e) {
            std::cerr << e.what() << "\n";
        }
        return true;
    }
};
IMPLEMENT_APP(MainApp);