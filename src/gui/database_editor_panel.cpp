#include "database_editor_panel.h"
#include "core/database/database_manager.h"
#include "gui/gui_components/horizontal_panel.h"

DatabaseEditorPanel::DatabaseEditorPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY)
{
    // INIT
    DatabaseManager& db_manager = DatabaseManager::getInstance();
    tb_manager_ = db_manager.get_tb_manager();
    // tb_manager_ = std::make_unique<DatabaseTableManager>(db_manger.get_current_database());

    // GUI
    HorizontalPanel* tb_selection_panel = new HorizontalPanel(this);
    wxStaticText* st = new wxStaticText(tb_selection_panel, wxID_ANY, "Current Table:");
    tb_selection_box_ = new wxChoice(tb_selection_panel, wxID_ANY);
    tb_selection_panel->add_children(st, tb_selection_box_);

    search_panel_ = new wxSearchPanel(this, tb_manager_);
    grid_panel_ = new wxDatabaseEditorGridPanel(this, tb_manager_);
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    refresh_tb_list();

    main_sizer->Add(tb_selection_panel, 0, wxALL, 10);
    main_sizer->Add(search_panel_, 0, wxEXPAND | wxALL, 10);
    main_sizer->Add(grid_panel_, 1, wxEXPAND | wxALL, 10);

    main_sizer->Fit(this);
    SetSizer(main_sizer);
    Layout();
    Refresh();

    // BIND
    tb_selection_box_->Bind(wxEVT_CHOICE, &DatabaseEditorPanel::on_select_table, this);

    // Bind the search event from the child panel to this panel's handler
    search_panel_->searched_value_tc_->Bind(wxEVT_COMMAND_TEXT_ENTER, &DatabaseEditorPanel::on_search, this);
    search_panel_->search_btn_->Bind(wxEVT_BUTTON, &DatabaseEditorPanel::on_search, this);
}

void DatabaseEditorPanel::refresh_tb_list() {
    // tb_selection_box
    if (!tb_selection_box_->IsEmpty()) tb_selection_box_->Clear();
    auto tb_names = tb_manager_->get_table_names();
    for (const auto& name : tb_names) {
        tb_selection_box_->Append(name);
    }
}

void DatabaseEditorPanel::on_search(wxCommandEvent& event) {
    auto sql = search_panel_->generate_search_sql();
    grid_panel_->update_search(sql);
}

void DatabaseEditorPanel::on_select_table(wxCommandEvent& event) {
    std::string selected_table_name = tb_selection_box_->GetStringSelection().ToStdString();
    tb_manager_->set_current_table_name(selected_table_name);
    grid_panel_->init_grid_cols();
    grid_panel_->update_grid();
}
