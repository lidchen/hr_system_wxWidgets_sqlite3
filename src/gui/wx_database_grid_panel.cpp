#include "wx_database_grid_panel.h"
#include "core/database/database_stmt_builder.h"
#include "wx_search_panel.h"

wxDatabaseGridPanel::wxDatabaseGridPanel(wxFrame* parent, Database* db) 
    : wxPanel(parent), db_(db)
{
    setup();
}

// This is really bad approach, grid_ has not been init
// Structure is a mess, lots of unnessary variable passing
void wxDatabaseGridPanel::setup() {
    ID_search_tc = wxWindow::NewControlId();
    ID_search_btn = wxWindow::NewControlId();

    // SEARCH PANEL
    search_panel_ = new wxPanel(this, wxID_ANY, wxPoint(-1, -1), wxDefaultSize);

    // SEARCH BAR
    wxPanel* search_bar = new wxPanel(search_panel_, wxID_ANY, wxPoint(-1, -1), wxDefaultSize);
    search_tc_ = new wxTextCtrl
        (search_bar, ID_search_tc, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    search_btn_ = new wxButton
        (search_bar, ID_search_btn, "Search");
    wxBoxSizer* search_bar_sizer = new wxBoxSizer(wxHORIZONTAL);
    search_bar_sizer->Add(search_tc_, 0, wxTOP, 10);
    search_bar_sizer->Add(search_btn_, 0, wxLEFT | wxTOP, 10);
    search_bar->SetSizer(search_bar_sizer);
    wxBoxSizer* search_sizer = new wxBoxSizer(wxVERTICAL);
    search_sizer->Add(search_bar);

    // SEARCH PREFER
    auto search_prefer_panel = new wxSearchPanel(search_panel_);
    search_sizer->Add(search_prefer_panel, 0, wxLEFT | wxTOP, 10);

    search_panel_->SetSizer(search_sizer);

    // GRID VIEW
    grid_ = new wxGrid(this, wxID_ANY);
    grid_manager_ = new DatabaseGridManager(grid_);

    grid_manager_->construct_by_result(db_->fetch_all("SELECT * FROM employee"));

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    main_sizer->Add(search_panel_, 0);
    main_sizer->Add(grid_, 1, wxTOP, 10);
    SetSizer(main_sizer);

    // Bind the search button specifically
    search_btn_->Bind(wxEVT_BUTTON, &wxDatabaseGridPanel::on_search, this);
    search_tc_->Bind(wxEVT_TEXT_ENTER, &wxDatabaseGridPanel::on_search, this);
}

void wxDatabaseGridPanel::on_search(wxCommandEvent& event) {
    try {
        std::string value = search_tc_->GetValue().ToStdString();
        // Escape single quotes in input to prevent SQL errors
        size_t pos = 0;
        while ((pos = value.find("'", pos)) != std::string::npos) {
            value.replace(pos, 1, "''");
            pos += 2;
        }
        DatabaseStmtBuilder builder;
        auto stmt = builder.select("*")
            .from("employee")
            .where()
            .column("FIRST_NAME").like(value).ignore_case()
            .build_with_or();
        grid_manager_->construct_by_result(
            db_->fetch_all(stmt)
        );
        Layout();
    } catch(DatabaseException e) {
        std::cerr << e.what() << "\n";
    }
}