#ifndef WX_SEARCH_PANEL_H_
#define WX_SEARCH_PANEL_H_
#include "wx/wx.h"
#include "wx/checkbox.h"
#include "wx/notebook.h"
#include "core/database/database_table_manager.h"
#include "gui/gui_components/cb_select_popup.h"

class wxSearchPanel : public wxPanel{
public:
    wxSearchPanel(wxPanel* parent, DatabaseTableManager* tb_manager);
    std::string generate_search_sql();
    // This is bad, really
    wxButton* search_btn_;
    wxTextCtrl* searched_value_tc_;
private:
    CbSelectPopup* col_select_popup_;
    DatabaseTableManager* tb_manager_;
    wxNotebook* nb_;
    wxCheckBox* cb_fuzzy_;
    wxPanel* basic_search_;
    wxPanel* advance_search_;
    void on_toggle(wxCommandEvent& event);
};

#endif // WX_SEARCH_PANEL_H_
