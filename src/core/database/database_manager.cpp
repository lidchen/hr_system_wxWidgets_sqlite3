#include "database_manager.h"
#include "gui/dialog/wx_get_folder_path_dialog.h"
#include "wx/dirdlg.h"
#include "wx/dir.h"
#include "wx/filename.h"

wxString DatabaseManager::get_database_dir() const {
    return dir_path_;
}
wxString DatabaseManager::generate_formatted_db_name(const wxString& db_raw_input_name) const {
    wxString formatted_db_name = db_raw_input_name;
    if (!db_raw_input_name.EndsWith(".sql")) {
        formatted_db_name += ".sql";
    }
    // Todo: replace space to _
    // ...
    return formatted_db_name;
}

wxString DatabaseManager::generate_full_db_name(const wxString& db_name) const {
    wxString full_db_name = dir_path_ 
                            + wxFileName::GetPathSeparator() 
                            + generate_formatted_db_name(db_name);
    return full_db_name;
}

void DatabaseManager::create_database(const wxString& db_full_name) {
    // wxString db_full_name = generate_full_db_name(db_raw_input_name);
    auto db = std::make_unique<Database>(db_full_name);
    database_container_.push_back(std::move(db));
}

void DatabaseManager::delete_database(const wxString& selected_db_name) {
    for (auto it = database_container_.begin(); it != database_container_.end(); ++it) {
        if ((*it)->filename_ == selected_db_name) {
            wxString full_selected_db_name = generate_full_db_name(selected_db_name);
            wxRemoveFile(full_selected_db_name); 
            database_container_.erase(it);
            break;
        }
    }
}

void DatabaseManager::set_current_database(const wxString selected_db_name) {
    if (current_database_ != nullptr) {
        current_database_->close();
        current_database_ = nullptr;
    }
    for (const auto& db_ptr : database_container_) {
        if (db_ptr->filename_ == selected_db_name) {
            current_database_ = db_ptr.get();
        }
    }
assert(current_database_ != nullptr && current_database_->filename_ == selected_db_name);
}

// void DatabaseManager::set_current_database_index(const int index) {
//     if (index >= 0 && index < database_container_.size()) {
//         current_db_index_ = index;
//     }
// }
void DatabaseManager::set_database_dir(const wxString& dir_path) {
    dir_path_ = dir_path;
}

Database* DatabaseManager::get_current_database() {
    return current_database_;
}
void DatabaseManager::scan_databases() {
    // TODO:
    // Shouldn't clear directly. Should check if there's db opened, throw error
    // Implement this in future
    database_container_.clear();
    wxDir dir(dir_path_);
    if (!dir.IsOpened()) {
        throw DatabaseException("cant open database dir");
    }

    wxString filename;
    bool cont = dir.GetFirst(&filename, "*.sql", wxDIR_FILES);
    while (cont) {
        wxString wx_db_path = dir_path_ + wxFileName::GetPathSeparator() + filename;
        auto db = std::make_unique<Database>(wx_db_path);
        database_container_.push_back(std::move(db));
        cont = dir.GetNext(&filename);
    }
}
std::vector<std::string> DatabaseManager::get_db_names() const {
    std::vector<std::string> db_names;
    for (const auto& db : database_container_) {
        db_names.push_back(db->filename_);
    }
    return db_names;
}

// void DatabaseManager::on_select_dir(wxCommandEvent& event) {
//     auto cb = [this](const wxString& path) {
//         dir_text_ctrl_->SetValue(path);
//     };
//     wxGetFolderPathDialog dir_dialog(nullptr, cb);
//     dir_dialog.ShowModal();
// }
// void set_database_dir(const std::string dir_path) {
//     dir_path_ = dir_path;
// }