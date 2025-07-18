#ifndef LIST_BOX_H_
#define LIST_BOX_H_

#include "wx/wx.h"

class Listbox : public wxPanel {
public:
    Listbox(wxWindow* parent);
    wxListBox* list_panel_;
    wxPanel* btn_panel_;
    void add_btn(wxButton* btn);
    void append_to_list(const wxString& info);
    void clear_list();
    void remove_selection();
    int get_selection();
    wxString get_selected_value();
    // void on_dbl_click(wxCommandEvent &event);
private:
    wxBoxSizer* btn_sizer_;
};

#endif // LIST_BOX_H_