#ifndef DATABASE_MANAGER_H_
#define DATABASE_MANAGER_H_
#include <string>
#include <vector>

#include "database.h"

#include "wx/wx.h"

class DatabaseManager {
public:
    static DatabaseManager& getInstance() {
        static DatabaseManager instance;
        return instance;
    }

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    // TODO: This one should call wxselectfolder
    wxString get_database_dir() const;
    wxString generate_formatted_db_name(const wxString &db_raw_input_name) const;
    wxString generate_full_db_name(const wxString &db_name) const;
    std::vector<std::string> get_db_names() const;

    void set_database_dir(const wxString& dir_path);
    void scan_databases();
    void create_database(const wxString &db_name);

    // Shouldn't put this feature in real application
    // I know its not good to indentify using string
    // But since same db_name cannot use twice
    // This could works now
    void delete_database(const wxString &db_name);
    void set_current_database(const wxString db_name);
    // void set_current_database_index(int index);

    void open_current_database();
    void close_current_database();
    // Returns a pointer to the current database, or nullptr if not set
    Database* get_current_database();
private:
    // Store all databases
    std::vector<std::unique_ptr<Database>> database_container_;
    Database* current_database_ = nullptr;
    std::string dir_path_ = "/Users/lid/Library/CloudStorage/OneDrive-Personal/Develop/work/hm_system/resources/sql";
    DatabaseManager() = default;
    ~DatabaseManager() = default;
};

#endif // DATABASE_MANAGER_H_