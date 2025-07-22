#include "wx_table_manager_dialog.h"
#include "core/database/database.h"

wxTableManagerDialog::wxTableManagerDialog(wxWindow* parent)
    : wxBaseDialog(parent, "Table Manager")
{
    db_ = db_manager_.get_current_database();
assert(db_->is_connected());
    tb_manager_ = std::make_unique<DatabaseTableManager>(db_);

    tb_listbox_ = new Listbox(content_panel_);
    wxButton* new_btn = new wxButton(tb_listbox_->btn_panel_, wxID_ANY, "New");
    wxButton* rename_btn = new wxButton(tb_listbox_->btn_panel_, wxID_ANY, "Rename");
    wxButton* delete_btn = new wxButton(tb_listbox_->btn_panel_, wxID_ANY, "Delete");
    tb_listbox_->add_btn(new_btn);
    tb_listbox_->add_btn(rename_btn);
    tb_listbox_->add_btn(delete_btn);

    content_sizer_->Add(0, 10);
    content_sizer_->Add(tb_listbox_, 1, wxEXPAND | wxALL, 0);
    content_sizer_->Add(0, 10);
    // content_panel_->SetSizer(content_sizer_);

    list_tables();

    new_btn->Bind(wxEVT_BUTTON, &wxTableManagerDialog::on_create_table, this);
    delete_btn->Bind(wxEVT_BUTTON, &wxTableManagerDialog::on_drop_selected_table, this);
}

void wxTableManagerDialog::list_tables() {
    auto tb_names = tb_manager_->get_table_names();
    for (const auto& tb_name : tb_names) {
        tb_listbox_->append_to_list(tb_name);
    }
}

void wxTableManagerDialog::on_create_table(wxCommandEvent& event) {
    wxTextEntryDialog dlg(this, wxT("Enter new table name:"), wxT("Create Table"));
    if (dlg.ShowModal() == wxID_OK) {
        wxString table_name = dlg.GetValue();
        try {
            tb_manager_->create_table(table_name);
            tb_listbox_->append_to_list(table_name);
        } catch (const DatabaseException& e) {
            wxLogError("Failed to create table: %s", e.what());
        }
    }
}

void wxTableManagerDialog::on_drop_selected_table(wxCommandEvent& event) {
    // By default do restrict drop
    wxString selected_tb_name = tb_listbox_->get_selected_value();
    tb_manager_->drop_table(selected_tb_name);
    tb_listbox_->remove_selection();
}

void wxTableManagerDialog::on_ok(wxCommandEvent& event) {

}

void wxTableManagerDialog::on_cancel(wxCommandEvent& event) {

}