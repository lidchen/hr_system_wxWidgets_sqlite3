#ifndef HORIZONTAL_PANEL_H_
#define HORIZONTAL_PANEL_H_

#include "wx/wx.h"

class HorizontalPanel : public wxPanel {
public:
    HorizontalPanel(wxWindow* parent)
        : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
        {}

    template<typename... Args>
    void add_children(Args... childern) 
    {
        static_assert((std::is_convertible_v<Args, wxWindow*> && ...), "All args should be wxWindow*");
        sizer_ = new wxBoxSizer(wxHORIZONTAL);
        (sizer_->Add(childern, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10), ...); 
        SetSizer(sizer_);
    }
private:
    wxBoxSizer* sizer_;
};

#endif // HORIZONTAL_PANEL_H_