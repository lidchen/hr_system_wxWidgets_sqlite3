#ifndef WX_DATABASE_MANAGER_PANEL_H_
#define WX_DATABASE_MANAGER_PANEL_H_
#include "wx_base_dialog.h"
#include "wx/wx.h"
#include "core/database/database_manager.h"
#include "gui/gui_components/listbox.h"

class wxDatabaseManagerDialog : public wxBaseDialog {
public:
    wxDatabaseManagerDialog(wxWindow* parent);
private:
    DatabaseManager& db_manager_ = DatabaseManager::getInstance();
    wxTextCtrl* db_dir_st_path_;
    Listbox* db_listbox_;
    int ID_database_name_;
    void list_database();
    void on_load_database(wxCommandEvent& event);
    void on_set_dir(wxCommandEvent& event);
    void on_create_database(wxCommandEvent &event);
    void on_delete_selected_database(wxCommandEvent& event);
    void on_ok(wxCommandEvent &event) override;
    void on_cancel(wxCommandEvent &event) override;
};

#endif // WX_CREATA_DATABASE_DIALOG
