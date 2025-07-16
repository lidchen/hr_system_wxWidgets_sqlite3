#ifndef WX_SEARCH_PANEL_H_
#define WX_SEARCH_PANEL_H_
#include "wx/wx.h"
#include "wx/checkbox.h"
#include "wx/notebook.h"

class wxSearchPanel : public wxPanel{
public:
    wxSearchPanel(wxPanel* parent);
private:
    wxNotebook* nb_;
    wxCheckBox* cb_fuzzy_;
    wxPanel* basic_search_;
    wxPanel* advance_search_;
    int ID_cb_fuzzy_;
    void on_toggle(wxCommandEvent& event);
};

#endif // WX_SEARCH_PANEL_H_
