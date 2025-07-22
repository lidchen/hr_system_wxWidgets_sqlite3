#include "wx_table_schema_dialog.h"
#include "wx/wx.h"
#include "wx/grid.h"

wxTableSchemaDialog::wxTableSchemaDialog(wxWindow* parent) 
    : wxBaseDialog(parent, "Table Schema") 
{
    DatabaseManager& db_manager = DatabaseManager::getInstance();
    tb_manager_ = std::make_unique<DatabaseTableManager>(db_manager.get_current_database());

    wxGrid* grid = new wxGrid(content_panel_, wxID_ANY);
    wxPanel* btn_panel = new wxPanel(content_panel_, wxID_ANY);
    grid->CreateGrid(10, 3);
    grid->HideRowLabels();
    grid->SetColLabelValue(0, "name");
    grid->SetColLabelValue(1, "type");
    grid->SetColLabelValue(2, "constraints");

    grid->SetColSize(0, 160);
    grid->SetColSize(1, 160);
    grid->SetColSize(2, 300);
    grid->DisableCellEditControl();
    grid->EnableEditing(false);

    wxBoxSizer* h_sizer = new wxBoxSizer(wxHORIZONTAL);
    h_sizer->Add(grid, 2, wxALL, 10);
    h_sizer->Add(btn_panel, 0, wxALL, 10);

    wxButton* add_btn = new wxButton(btn_panel, wxID_ANY, "add");
    wxButton* alter_btn = new wxButton(btn_panel, wxID_ANY, "alter");
    wxButton* delete_btn = new wxButton(btn_panel, wxID_ANY, "delete");
    wxBoxSizer* btn_sizer = new wxBoxSizer(wxVERTICAL);
    btn_sizer->Add(add_btn, 0, wxALL, 10);
    btn_sizer->Add(alter_btn, 0, wxALL, 10);
    btn_sizer->Add(delete_btn, 0, wxALL, 10);
    btn_panel->SetSizer(btn_sizer);

    content_panel_->SetSizer(h_sizer);
    content_panel_->Layout();

    // Get actual size of grid after sizer/layout
    content_panel_->Layout(); // Ensure layout is updated
}

void wxTableSchemaDialog::list_schemas() {
}