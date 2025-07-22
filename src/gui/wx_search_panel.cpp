#include "wx_search_panel.h"

wxSearchPanel::wxSearchPanel(wxPanel* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize) 
{
    ID_cb_fuzzy_ = wxWindow::NewControlId();
    nb_ = new wxNotebook(this, wxID_ANY, wxPoint(-1, -1), wxDefaultSize, wxNB_TOP);

    // BASIC SEARCH
    basic_search_ = new wxPanel(nb_, wxID_ANY, wxPoint(-1, -1), wxDefaultSize);
    wxBoxSizer* basic_sizer = new wxBoxSizer(wxHORIZONTAL);
    cb_fuzzy_ = new wxCheckBox
        (basic_search_, ID_cb_fuzzy_, "fuzzy_search", wxDefaultPosition, wxDefaultSize);
    cb_fuzzy_->SetValue(true);
    basic_sizer->Add(cb_fuzzy_);
    basic_search_->SetSizer(basic_sizer);

    // ADVANCE SEARCH
    advance_search_ = new wxPanel(nb_, wxID_ANY, wxPoint(-1, -1), wxDefaultSize);
    wxBoxSizer* advance_sizer = new wxBoxSizer(wxVERTICAL);
    advance_search_->SetSizer(advance_sizer);

    nb_->AddPage(basic_search_, wxT("basic_search"));
    nb_->AddPage(advance_search_, wxT("advance_search"));

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    main_sizer->Add(nb_);
    SetSizer(main_sizer);
    Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &wxSearchPanel::on_toggle, this, ID_cb_fuzzy_);
}

void wxSearchPanel::on_toggle(wxCommandEvent& event) {
    wxCheckBox* cb = dynamic_cast<wxCheckBox*>(event.GetEventObject());
    if (cb) {
    }
}