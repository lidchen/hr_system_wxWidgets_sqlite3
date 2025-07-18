#ifndef WX_DATABASE_GRID_PANEL_H_
#define WX_DATABASE_GRID_PANEL_H_

#include "wx/wx.h"
#include "wx/grid.h"
#include "database_grid_manager.h"
#include "core/database/database.h"

class wxDatabaseGridPanel: public wxPanel {
public:
    wxDatabaseGridPanel(wxFrame* parent, Database* db);
    wxDatabaseGridPanel(const wxDatabaseGridPanel& other) = delete;
    wxDatabaseGridPanel& operator= (const wxDatabaseGridPanel& other) = delete;
    ~wxDatabaseGridPanel() = default;
    void on_search(wxCommandEvent& event);
    void setup();
private:
// TODO
    struct SearchPrefer {
        bool search_first_name;
        bool search_last_name;
        bool search_sex;
    };
// TODO
// Should use smart point here
    DatabaseGridManager* grid_manager_;
    wxPanel* main_panel_;
    wxPanel* search_panel_;
    wxPanel* grid_panel_;
    wxGrid* grid_;

    wxTextCtrl* search_tc_;
    wxButton* search_btn_;

    Database* db_;
    Database* db_backup_;
    int ID_search_tc;
    int ID_search_btn;
};

#endif // WX_DATABASE_GRID_PANEL_H_