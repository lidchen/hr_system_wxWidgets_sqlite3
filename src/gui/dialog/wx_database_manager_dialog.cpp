#include "wx_database_manager_dialog.h"
#include "core/database/database_manager.h"
#include "gui/dialog/wx_get_folder_path_dialog.h"
#include "gui/gui_components/labeled_text_ctrl.h"
#include "gui/gui_components/horizontal_panel.h"
#include "gui/gui_components/listbox.h"

wxDatabaseManagerDialog::wxDatabaseManagerDialog(wxWindow* parent)
    : wxBaseDialog(parent, "Manage Database")
{
    ID_database_name_ = wxWindow::NewControlId();

    // DB DIR HANDLE
    HorizontalPanel* db_dir_panel = new HorizontalPanel(content_panel_);
    wxStaticText* db_dir_st = new wxStaticText(db_dir_panel, wxID_ANY, wxT("Current db path: "));
    db_dir_st_path_ = new wxTextCtrl(db_dir_panel, wxID_ANY
                                    , db_manager_.get_database_dir(), wxDefaultPosition
                                    , wxDefaultSize, wxTE_READONLY | wxBORDER_NONE);

    wxButton* db_dir_btn = new wxButton(content_panel_, wxID_ANY, "set dir");
    db_dir_btn->Bind(wxEVT_BUTTON, &wxDatabaseManagerDialog::on_set_dir, this);
    db_dir_panel->add_children(db_dir_st, db_dir_st_path_);

    // LIST BOX SHOW DB IN DB_DIR
    db_listbox_ = new Listbox(content_panel_);
    wxButton* new_btn = new wxButton(db_listbox_->btn_panel_, wxID_ANY, "New");
    wxButton* rename_btn = new wxButton(db_listbox_->btn_panel_, wxID_ANY, "Rename");
    wxButton* delete_btn = new wxButton(db_listbox_->btn_panel_, wxID_ANY, "Delete");
    db_listbox_->add_btn(new_btn);
    db_listbox_->add_btn(rename_btn);
    db_listbox_->add_btn(delete_btn);
    
    wxBoxSizer* content_sizer = new wxBoxSizer(wxVERTICAL);
    content_sizer->Add(0, 10); 
    content_sizer->Add(db_dir_btn);
    content_sizer->Add(0, 10); 
    content_sizer->Add(db_dir_panel);
    content_sizer->Add(0, 10);
    content_sizer->Add(db_listbox_, 1, wxEXPAND | wxALL, 0);

    content_panel_->SetSizer(content_sizer);

// TODO: Test when dir is null
    list_database();

    new_btn->Bind(wxEVT_BUTTON, &wxDatabaseManagerDialog::on_create_database, this);
    delete_btn->Bind(wxEVT_BUTTON, &wxDatabaseManagerDialog::on_delete_selected_database, this);

    Bind(wxEVT_BUTTON, &wxDatabaseManagerDialog::on_ok, this, wxID_OK);
    Bind(wxEVT_BUTTON, &wxDatabaseManagerDialog::on_cancel, this, wxID_CANCEL);
    db_listbox_->Bind(wxEVT_LISTBOX_DCLICK, &wxDatabaseManagerDialog::on_db_click_select, this);
}

// call this after set dir
void wxDatabaseManagerDialog::list_database() {
    db_listbox_->clear_list();
    db_manager_.scan_databases();
    auto db_names = db_manager_.get_db_names();
    for (auto db_name : db_names) {
        db_listbox_->append_to_list(db_name);
    }
}

Database* wxDatabaseManagerDialog::get_selected_database() {
    return db_manager_.get_current_database();
}

bool wxDatabaseManagerDialog::set_selected_database() {
    auto selected_db_name = db_listbox_->get_selected_value();
    if(!selected_db_name.empty()) {
        db_manager_.set_current_database(selected_db_name);
        return true;
    } else {
        wxLogError("Please select a database");
        return false;
    }
}

void wxDatabaseManagerDialog::on_set_dir(wxCommandEvent& event) {
    auto cb = [this](const wxString& path) {
        db_dir_st_path_->SetLabel(path);
        db_manager_.set_database_dir(path);
        content_panel_->GetSizer()->Layout();
        // Call on_list_database after setting the directory
        list_database();
    };
    wxGetFolderPathDialog* dir_dialog = new wxGetFolderPathDialog(nullptr, cb);
    dir_dialog->ShowModal();
}

// Bind this with new button
void wxDatabaseManagerDialog::on_create_database(wxCommandEvent& event) {
    wxTextEntryDialog dlg(this, wxT("Enter new database name:"), wxT("Create Database"));
    if (dlg.ShowModal() == wxID_OK) {
        wxString db_raw_input_name = dlg.GetValue();
        wxString db_formatted_name = db_manager_.generate_formatted_db_name(db_raw_input_name);
// Todo: 
// Check if raw input just all space or contains . or something illegal

        // Check if have same name's sql
        auto db_names = db_manager_.get_db_names();
        for (const auto& name : db_names) {
            if (name == db_formatted_name) {
                wxMessageBox("A database with this name already exists.", "Error", wxICON_ERROR);
                return;
            }
        }

        wxString db_full_name = db_manager_.generate_full_db_name(db_raw_input_name);
        db_manager_.create_database(db_full_name);
        // list_database();
        db_listbox_->append_to_list(db_formatted_name);
    }
}
void wxDatabaseManagerDialog::on_delete_selected_database(wxCommandEvent& event) {
    std::string selected_db_name = db_listbox_->get_selected_value();
    int res = wxMessageBox(
        wxString::Format("Are you sure you want to delete database '%s'?", selected_db_name),
        "Confirm Delete", wxYES_NO | wxICON_QUESTION, this);
    if (res != wxYES)
        return;
    try {
        wxString selected_db_name = db_listbox_->get_selected_value();
        db_manager_.delete_database(selected_db_name);
        db_listbox_->remove_selection();
    } catch (const DatabaseException& e) {
        wxLogError("Can delete database: %s", e.what());
    }
}
void wxDatabaseManagerDialog::on_db_click_select(wxCommandEvent& event) {
    if (set_selected_database()) {
        EndModal(wxID_OK);
    }
}
void wxDatabaseManagerDialog::on_ok(wxCommandEvent& event) {
    if(set_selected_database()) {
        EndModal(wxID_OK);
    }
}

void wxDatabaseManagerDialog::on_cancel(wxCommandEvent& event) {
    EndModal(wxID_CANCEL);
}