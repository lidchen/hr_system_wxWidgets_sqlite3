#ifndef WX_DATABASE_EDITOR_PANEL_H_
#define WX_DATABASE_EDITOR_PANEL_H_

#include "wx/wx.h"
#include "gui/gui_components/grid_panel/wx_database_editor_grid_panel.h"

class wxDatabaseEditorPanel : public wxPanel {
public:
    wxDatabaseEditorPanel(wxWindow* parent);
    void on_select_table(wxCommandEvent& event);
    wxSize get_size();
private:
    std::unique_ptr<DatabaseTableManager> tb_manager_;
    wxChoice* tb_selection_box_;
    wxDatabaseEditorGridPanel* grid_panel_;
};

#endif // WX_DATABASE_EDITOR_PANEL_H_