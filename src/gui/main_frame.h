#ifndef MAIN_FRAME_H_
#define MAIN_FRAME_H_

#include "wx/wx.h"
#include "core/database/database.h"

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &name);
    void show_welcome_panel();
    void show_view_panel();
    void show_database_manager();
    void show_table_manager();
private:
    wxPanel* current_panel_ = nullptr;
    std::string default_project_path = "/Users/lid/Library/CloudStorage/OneDrive-Personal/Develop/work/hm_system/";
    wxMenuBar *menubar;
    wxMenu* file;
    wxMenu* import_from_csv;
    wxMenu* database_manager;
    void on_db_manager(wxCommandEvent& event);
    void on_tb_manager(wxCommandEvent& event);
    void on_quit(wxCommandEvent &event);
};

#endif // MAIN_FRAME_H_