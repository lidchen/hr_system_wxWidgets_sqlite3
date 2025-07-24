#include "main_frame.h"
#include "gui/welcome_panel.h"
#include "gui/view_panel.h"
#include "gui/dialog/wx_database_manager_dialog.h"
#include "gui/dialog/wx_table_manager_dialog.h"
#include "gui/dialog/wx_create_table_dialog.h"

MainFrame::MainFrame(const wxString& name) 
    : wxFrame(nullptr, wxID_ANY, name, wxDefaultPosition, wxSize(700, 500))
{
    DatabaseManager& db_mananger = DatabaseManager::getInstance();
    db_mananger.init();

    menubar = new wxMenuBar();
    file = new wxMenu();
    import_from_csv = new wxMenu();
    database_manager = new wxMenu();

    wxMenuItem* open_db_mananger = new wxMenuItem(database_manager, wxID_ANY, wxT("Open Database Manager"));
    wxMenuItem* open_tb_manager = new wxMenuItem(database_manager, wxID_ANY, wxT("Open Table Manager"));
    database_manager->Append(open_db_mananger);
    database_manager->Append(open_tb_manager);

    file->AppendSubMenu(database_manager, wxT("Manage Database"));
    file->AppendSubMenu(import_from_csv, wxT("Import From CSV"));
    menubar->Append(file, wxT("File"));
    SetMenuBar(menubar);

    Bind(wxEVT_MENU, &MainFrame::on_db_manager, this, open_db_mananger->GetId());
    Bind(wxEVT_MENU, &MainFrame::on_tb_manager, this, open_tb_manager->GetId());

    // Open WelcomePanel at default
    wxCreateTableDialog* test = new wxCreateTableDialog(this);
    test->ShowModal();
}
void MainFrame::on_db_manager(wxCommandEvent& event) {
    wxDatabaseManagerDialog* db_manager_dialog = new wxDatabaseManagerDialog(this);
    db_manager_dialog->ShowModal();
    db_manager_dialog->Destroy();
}
void MainFrame::on_tb_manager(wxCommandEvent& event) {
    wxTableManagerDialog* tb_manager_dialog = new wxTableManagerDialog(this);
    tb_manager_dialog->ShowModal();
    tb_manager_dialog->Destroy();
}
void MainFrame::show_welcome_panel() {
    if (current_panel_) current_panel_->Destroy();
    current_panel_ = new WelcomePanel(this);
}
void MainFrame::show_view_panel() {
    if (current_panel_) current_panel_->Destroy();
    try {
        current_panel_ = new ViewPanel(this);
    } catch (DatabaseException e) {
        wxLogError(e.what());
    }
}