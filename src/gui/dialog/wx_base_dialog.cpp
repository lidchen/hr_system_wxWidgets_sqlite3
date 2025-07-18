#include "wx_base_dialog.h"

wxBaseDialog::wxBaseDialog(wxWindow* parent, const wxString& name)
    : wxDialog(parent, wxID_ANY, name, wxDefaultPosition, wxSize(800, 600))
{
    wxPanel* btn_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    wxButton* ok_btn = new wxButton(btn_panel, wxID_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize);
    wxButton* cancel_btn = new wxButton(btn_panel, wxID_CANCEL, wxT("CANCEL"), wxDefaultPosition, wxDefaultSize);
    wxBoxSizer* btn_sizer = new wxBoxSizer(wxHORIZONTAL);
    btn_sizer->AddStretchSpacer(1);
    btn_sizer->Add(ok_btn, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
    btn_sizer->Add(cancel_btn, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
    btn_sizer->AddStretchSpacer(1);
    btn_panel->SetSizer(btn_sizer);

    content_panel_ = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    main_sizer->Add(content_panel_, 1, wxEXPAND | wxALL, 10);
    main_sizer->Add(btn_panel, 0, wxEXPAND | wxALL, 10);
    SetSizer(main_sizer);

    // Bind default handlers (can be overridden by child)
    Bind(wxEVT_BUTTON, &wxBaseDialog::on_ok, this, wxID_OK);
    Bind(wxEVT_BUTTON, &wxBaseDialog::on_cancel, this, wxID_CANCEL);

    // Allow child to override by re-binding in its constructor
    // Example for child: Bind(wxEVT_BUTTON, &ChildDialog::on_ok, this, wxID_OK);

    // EndModal for OK/CANCEL buttons
    Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { EndModal(wxID_OK); }, wxID_OK);
    Bind(wxEVT_BUTTON, [this](wxCommandEvent&) { EndModal(wxID_CANCEL); }, wxID_CANCEL);
    Bind(wxEVT_CLOSE_WINDOW, [this](wxCloseEvent&) { EndModal(wxID_CANCEL); });
}
void wxBaseDialog::on_ok(wxCommandEvent& event){
    if (auto* btn = dynamic_cast<wxButton*>(event.GetEventObject())) {
        btn->SetFocus();
    }
}
void wxBaseDialog::on_cancel(wxCommandEvent& event) {
    if (auto* btn = dynamic_cast<wxButton*>(event.GetEventObject())) {
        btn->SetFocus();
    }
}