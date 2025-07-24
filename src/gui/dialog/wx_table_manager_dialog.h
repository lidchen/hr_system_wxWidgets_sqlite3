#ifndef WX_TABLE_MANAGER_DIALOG_H_
#define WX_TABLE_MANAGER_DIALOG_H_

#include "wx_base_dialog.h"
#include "core/database/database_table_manager.h"
#include "core/database/database_manager.h"
#include "gui/gui_components/listbox.h"

#include "wx/wx.h"

class wxTableManagerDialog : public wxBaseDialog {
public:
    wxTableManagerDialog(wxWindow* parent);
private:
    DatabaseManager& db_manager_ = DatabaseManager::getInstance();
    std::unique_ptr<DatabaseTableManager> tb_manager_;
    Listbox* tb_listbox_;
    Database* db_;
    void show_schema_dialog();
    void list_tables();
    void on_create_table(wxCommandEvent& event);
    void on_load_tables(wxCommandEvent& event);
    void on_drop_selected_table(wxCommandEvent& event);
    void on_db_click_select(wxCommandEvent& event);
    void on_select(wxCommandEvent& event);
    void on_ok(wxCommandEvent& event) override;
    void on_cancel(wxCommandEvent& event) override;
};

#endif // WX_TABLE_MANAGER_DIALOG_H_
