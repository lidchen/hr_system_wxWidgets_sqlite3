#ifndef DATABASE_EDITOR_PANEL_H_
#define DATABASE_EDITOR_PANEL_H_

#include "wx/wx.h"
#include "gui/gui_components/grid_panel/wx_database_editor_grid_panel.h"
#include "gui/gui_components/wx_search_panel.h"

class DatabaseEditorPanel : public wxPanel {
public:
    DatabaseEditorPanel(wxWindow* parent);
    void refresh_tb_list();
    void on_select_table(wxCommandEvent& event);
    void on_search(wxCommandEvent& event);
private:
    DatabaseTableManager* tb_manager_;
    wxChoice* tb_selection_box_;
    wxSearchPanel* search_panel_;
    wxDatabaseEditorGridPanel* grid_panel_;
};

#endif // DATABASE_EDITOR_PANEL_H_