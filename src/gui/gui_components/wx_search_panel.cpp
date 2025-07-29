#include "wx_search_panel.h"
#include "gui/gui_components/horizontal_panel.h"
#include "gui/gui_components/cb_select_popup.h"
#include "core/database/database_stmt_builder.h"

wxSearchPanel::wxSearchPanel(wxPanel* parent, DatabaseTableManager* tb_manager) 
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
    , tb_manager_(tb_manager) 
{
    nb_ = new wxNotebook(this, wxID_ANY, wxPoint(-1, -1), wxDefaultSize, wxNB_TOP);

    // BASIC SEARCH
    basic_search_ = new wxPanel(nb_, wxID_ANY, wxPoint(-1, -1), wxDefaultSize);
    wxBoxSizer* basic_sizer = new wxBoxSizer(wxVERTICAL);

    HorizontalPanel* search_bar = new HorizontalPanel(basic_search_);
    searched_value_tc_ = new wxTextCtrl(search_bar, wxID_ANY, "");
    search_btn_ = new wxButton(search_bar, wxID_ANY, "Search");
    search_bar->add_children(searched_value_tc_, search_btn_);

    wxPanel* toggle_panel = new wxPanel(basic_search_, wxID_ANY);
    wxBoxSizer* toggle_panel_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* col_select_btn = new wxButton(toggle_panel, wxID_ANY, "Select Column");

    auto col_names = tb_manager_->get_current_table_schema().get_col_names();
    col_select_popup_ = new CbSelectPopup(wxGetTopLevelParent(toggle_panel), col_names);

    cb_fuzzy_ = new wxCheckBox
        (toggle_panel, wxID_ANY, "fuzzy_search", wxDefaultPosition, wxDefaultSize);
    cb_fuzzy_->SetValue(true);
    toggle_panel_sizer->Add(cb_fuzzy_);
    toggle_panel_sizer->Add(col_select_btn);
    toggle_panel->SetSizer(toggle_panel_sizer);

    basic_sizer->Add(search_bar, 1, wxALL, 10);
    basic_sizer->Add(toggle_panel);
    basic_search_->SetSizer(basic_sizer);

    // ADVANCE SEARCH
    advance_search_ = new wxPanel(nb_, wxID_ANY, wxPoint(-1, -1), wxDefaultSize);
    wxBoxSizer* advance_sizer = new wxBoxSizer(wxVERTICAL);
    advance_search_->SetSizer(advance_sizer);

    nb_->AddPage(basic_search_, wxT("basic_search"));
    nb_->AddPage(advance_search_, wxT("advance_search"));

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    main_sizer->Add(nb_, 1, wxEXPAND);
    SetSizer(main_sizer);

    // BIND 
    cb_fuzzy_->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &wxSearchPanel::on_toggle, this);

    // COL SELECTION POPUP FOR BASIC SEARCH
    col_select_btn->Bind(wxEVT_BUTTON, [=, this](wxCommandEvent&) {
        auto col_names = tb_manager_->get_current_table_schema().get_col_names();
        // CbSelectPopup* col_select_popup = new CbSelectPopup(wxGetTopLevelParent(toggle_panel), col_names);

        // Get the button's position and size
        wxPoint btn_pos = col_select_btn->GetPosition();
        wxSize btn_size = col_select_btn->GetSize();
        // The point just below the button in its parent's coordinates
        wxPoint below_btn(btn_pos.x, btn_pos.y + btn_size.GetHeight());
        // Convert that point to screen coordinates
        wxPoint screen_pos = col_select_btn->GetParent()->ClientToScreen(below_btn);
        // Position the popup there
        col_select_popup_->Position(screen_pos, wxSize(0, 0));
        col_select_popup_->Popup();
    });
}

std::string wxSearchPanel::generate_search_sql() {
    // BASIC SEARCH
    if (nb_->GetCurrentPage() == basic_search_) {
        auto selection_info = col_select_popup_->get_selection();
        std::string search_value = searched_value_tc_->GetValue().ToStdString();
        bool fuzzy_on = cb_fuzzy_->GetValue();

        DatabaseStmtBuilder builder;
        auto where_builder = builder.select("*")
            .from(tb_manager_->get_current_table_name())
            .where();

        auto col_names = tb_manager_->get_current_table_schema().get_col_names();
        for (size_t i = 0; i < col_names.size(); ++i) {
            if (selection_info[i]) {
                where_builder.column(col_names[i]);
                if (fuzzy_on) {
                    where_builder.like(search_value);
                } else {
                    where_builder.equals(search_value);
                }
            }
        }
        auto sql = where_builder.build_with_or();
        return sql;
    }
    return "";
}

void wxSearchPanel::on_toggle(wxCommandEvent& event) {
    wxCheckBox* cb = dynamic_cast<wxCheckBox*>(event.GetEventObject());
    if (cb) {
    }
}