#include "wx/wx.h"
#include "database_grid_manager.h"
#include "wx_database_grid_panel.h"
#include "core/database/database.h"
#include "core/database/database_exception.h"
#include "core/database/database_stmt_builder.h"
#include "dialog/wx_database_manager_dialog.h"
#include "dialog/wx_get_folder_path_dialog.h"

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
    std::string project_path = "/Users/lid/Library/CloudStorage/OneDrive-Personal/Develop/work/hm_system/";
        try{
            // DatabaseStmtBuilder builder;
            // Database* db = new Database("employee");
            // db->begin_transaction();

            // builder.create_table("employee")
            // .add_column("ID INT PRIMARY KEY NOT NULL")
            // .add_column("FIRST_NAME TEXT")
            // .add_column("LAST_NAME TEXT")
            // .add_column("SEX TEXT")
            // .add_column("EMAIL TEXT")
            // .add_column("PHONE TEXT")
            // .add_column("BIRTH DATE")
            // .add_column("TITLE TEXT");
            // db->execute_sql(builder.build());
            // db->file_handler_->import_data_from_csv("employee", project_path + "/resources/employee.csv");
            // CLIUtil::print_table(db, "employee");
            // CLIUtil::print_query_result(result);

            wxFrame* main_frame = new wxFrame(nullptr, wxID_ANY, "test", wxDefaultPosition, wxSize(700, 500));
// TEST
// wxCreateDatabaseDialog* test_dialog = new wxCreateDatabaseDialog(main_frame);
auto* test = new wxDatabaseManagerDialog(main_frame);
test->ShowModal();

            // DatabaseGridManager* db_grid = new DatabaseGridManager(main_frame, &db);
            // wxDatabaseGridPanel* grid_panel = new wxDatabaseGridPanel(main_frame, db);

            main_frame->Show(true);
            // db.rollback_transaction();
        } catch(DatabaseException e) {
            std::cerr << e.what() << "\n";
        }
        return true;
    }
};
IMPLEMENT_APP(MyApp);