#include "wx_database_editor_panel.h"
#include "core/database/database_manager.h"
#include "gui/gui_components/horizontal_panel.h"

wxDatabaseEditorPanel::wxDatabaseEditorPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY)
{
    // INIT
    DatabaseManager& db_manger = DatabaseManager::getInstance();
    tb_manager_ = std::make_unique<DatabaseTableManager>(db_manger.get_current_database());

    // GUI
    HorizontalPanel* tb_selection_panel = new HorizontalPanel(this);
    wxStaticText* st = new wxStaticText(tb_selection_panel, wxID_ANY, "Current Table:");
    tb_selection_box_ = new wxChoice(this, wxID_ANY);
    search_panel_ = new wxSearchPanel(this, tb_manager_.get());
    grid_panel_ = new wxDatabaseEditorGridPanel(this, tb_manager_.get());
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    // tb_selection_box
    auto tb_names = tb_manager_->get_table_names();
    for (const auto& name : tb_names) {
        tb_selection_box_->Append(name);
    }
    tb_selection_panel->add_children(st, tb_selection_box_);

    main_sizer->Add(tb_selection_panel, 0, wxALL, 10);
    main_sizer->Add(search_panel_, 0, wxEXPAND | wxALL, 10);
    main_sizer->Add(grid_panel_, 1, wxEXPAND | wxALL, 10);

    main_sizer->Fit(this);
    SetSizer(main_sizer);
    Layout();
    Refresh();

    // BIND
    tb_selection_box_->Bind(wxEVT_CHOICE, &wxDatabaseEditorPanel::on_select_table, this);

    // Bind the search event from the child panel to this panel's handler
    search_panel_->searched_value_tc_->Bind(wxEVT_COMMAND_TEXT_ENTER, &wxDatabaseEditorPanel::on_search, this);
    search_panel_->search_btn_->Bind(wxEVT_BUTTON, &wxDatabaseEditorPanel::on_search, this);
}

void wxDatabaseEditorPanel::on_search(wxCommandEvent& event) {
    auto sql = search_panel_->generate_search_sql();
    std::cout << sql << "\n";
    grid_panel_->update_search(sql);
}

void wxDatabaseEditorPanel::on_select_table(wxCommandEvent& event) {
    std::string selected_table_name = tb_selection_box_->GetStringSelection().ToStdString();
    tb_manager_->set_current_table_name(selected_table_name);
    grid_panel_->init_grid_cols();
    grid_panel_->update_grid();
}
