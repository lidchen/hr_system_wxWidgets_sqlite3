#include "cb_select_popup.h"

CbSelectPopup::CbSelectPopup(wxWindow* parent, std::vector<std::string> cb_names) 
    : wxPopupTransientWindow(parent, wxBORDER_SIMPLE)
{
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* panelSizer = new wxBoxSizer(wxVERTICAL);
    for (const auto& name : cb_names) {
        wxCheckBox* cb = new wxCheckBox(panel, wxID_ANY, name);
        cb_container_.push_back(cb);
        panelSizer->Add(cb, 0, wxALL, 5);
    }
    panel->SetSizerAndFit(panelSizer);

    wxBoxSizer* popupSizer = new wxBoxSizer(wxVERTICAL);
    popupSizer->Add(panel, 1, wxEXPAND | wxALL, 0);
    SetSizerAndFit(popupSizer);
}
std::vector<bool> CbSelectPopup::get_selection() {
    std::vector<bool> selection_info;
    for (auto const& cb : cb_container_) {
        selection_info.push_back(cb->GetValue());
    }
    return selection_info;
}