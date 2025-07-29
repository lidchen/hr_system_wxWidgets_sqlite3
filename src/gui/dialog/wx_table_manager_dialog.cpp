#include <assert.h>
#include "wx_table_manager_dialog.h"

#include "core/database/database.h"
#include "wx_table_schema_dialog.h"
#include "wx_create_table_dialog.h"

wxTableManagerDialog::wxTableManagerDialog(wxWindow* parent)
    : wxBaseDialog(parent, "Table Manager")
{
    db_ = db_manager_.get_current_database();
    tb_manager_ = db_manager_.get_tb_manager();

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
    delete_btn->Bind(wxEVT_BUTTON, &wxTableManagerDialog::on_delete_table, this);
    rename_btn->Bind(wxEVT_BUTTON, &wxTableManagerDialog::on_rename, this);
    tb_listbox_->list_panel_->Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &wxTableManagerDialog::on_select, this);
    tb_listbox_->list_panel_->Bind(wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, &wxTableManagerDialog::on_db_click_select, this);

}

void wxTableManagerDialog::list_tables() {
    tb_listbox_->clear_list();
    auto tb_names = tb_manager_->get_table_names();
    for (const auto& tb_name : tb_names) {
        tb_listbox_->append_to_list(tb_name);
    }
}

void wxTableManagerDialog::on_create_table(wxCommandEvent& event) {
    wxCreateTableDialog* create_tb_dlg = new wxCreateTableDialog(this);
    if (create_tb_dlg->ShowModal() == wxID_OK) {
        try {
            list_tables();
        } catch (const DatabaseException& e) {
            wxLogError("Failed to create table: %s", e.what());
        }
    } 
    // wxTextEntryDialog dlg(this, wxT("Enter new table name:"), wxT("Create Table"));
    // if (dlg.ShowModal() == wxID_OK) {
    //     std::string table_name = dlg.GetValue().ToStdString();
    //     try {
    //         wxTableSchemaDialog* tb_schema_dlg = new wxTableSchemaDialog(this);
    //         if (tb_schema_dlg->ShowModal() == wxID_OK) {
    //         }
    //         tb_manager_->create_table(table_name);
    //         tb_listbox_->append_to_list(table_name);
    //         // tb_manager_->set_current_table_name(table_name.ToStdString());
    //         // show_schema_dialog();
    //     } catch (const DatabaseException& e) {
    //         wxLogError("Failed to create table: %s", e.what());
    //     }
    // }
}
void wxTableManagerDialog::show_schema_dialog() {
    wxTableSchemaDialog* schema_dlg = new wxTableSchemaDialog(this);
    if(schema_dlg->ShowModal() == wxID_OK) {
        list_tables();
    }
    schema_dlg->Destroy();
}

void wxTableManagerDialog::on_select(wxCommandEvent& event) {
    std::string selected_tb_name = tb_listbox_->get_selected_value();
    tb_manager_->set_current_table_name(selected_tb_name);
}

void wxTableManagerDialog::on_db_click_select(wxCommandEvent& event) {
    std::string selected_tb_name = tb_listbox_->get_selected_value();
    tb_manager_->set_current_table_name(selected_tb_name);
    show_schema_dialog();
}

void wxTableManagerDialog::on_rename(wxCommandEvent& event) {
    try {
        std::string current = tb_listbox_->get_selected_value();
        wxTextEntryDialog dlg(this, "Rename Table to:", "Rename Table", current);
        if (dlg.ShowModal() == wxID_OK) {
            std::string new_name = dlg.GetValue().ToStdString();
            tb_manager_->rename_table(current, new_name);
            int sel_index = tb_listbox_->get_selection();
            assert(sel_index != wxNOT_FOUND);
            tb_listbox_->rename_selection(new_name);
        }
    } catch (const DatabaseException& e) {
        wxLogError("Failed to rename table: %s", e.what());
    }
}

void wxTableManagerDialog::on_delete_table(wxCommandEvent& event) {
    std::string selected_value = tb_listbox_->get_selected_value();
    int res = wxMessageBox(
        wxString::Format("Are you sure you want to delete table '%s'?", selected_value),
        "Confirm Delete", wxYES_NO | wxICON_QUESTION, this);
    if (res != wxYES)
        return;
    try {
        tb_manager_->drop_table(selected_value);
        tb_listbox_->remove_selection();
    } catch (const DatabaseException& e) {
        wxLogError("Can't Delete Table: %s", e.what());
    }
}

void wxTableManagerDialog::on_ok(wxCommandEvent& event) {

}

void wxTableManagerDialog::on_cancel(wxCommandEvent& event) {

}