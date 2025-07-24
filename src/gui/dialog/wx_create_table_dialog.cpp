#include "wx_create_table_dialog.h"
#include "gui/gui_components/horizontal_panel.h"
#include "gui/gui_components/labeled_text_ctrl.h"

wxCreateTableDialog::wxCreateTableDialog(wxWindow* parent)
    : wxBaseDialog(parent, "Create Table")
{
    // INIT STUFF
    DatabaseManager& db_mananger = DatabaseManager::getInstance();
    tb_manager_ = std::make_unique<DatabaseTableManager>(db_mananger.get_current_database());

    LabeledTextCtrl* tb_name_panel = new LabeledTextCtrl(content_panel_, wxID_ANY, "new table name: ", "", wxDefaultPosition, wxDefaultSize);

    // GUI
    wxPanel* h_panel = new wxPanel(content_panel_, wxID_ANY);
    grid_ = new wxGrid(h_panel, wxID_ANY);
    wxPanel* btn_panel = new wxPanel(h_panel, wxID_ANY);
    wxBoxSizer* h_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* btn_sizer = new wxBoxSizer(wxVERTICAL);
    wxButton* rearrange_btn = new wxButton(btn_panel, wxID_ANY, "Rearrange");
    wxButton* delete_btn = new wxButton(btn_panel, wxID_ANY, "Delete");

    wxBoxSizer* grid_sizer = new wxBoxSizer(wxHORIZONTAL);
    grid_->CreateGrid(default_row_num_, 3);
    construct_grid();
    grid_sizer->Add(grid_, 1, wxEXPAND);

    btn_sizer->Add(rearrange_btn, 0, wxALL, 10);
    btn_sizer->Add(delete_btn, 0, wxALL, 10);
    btn_panel->SetSizer(btn_sizer);
    h_panel->SetSizer(h_sizer);
    h_sizer->Add(grid_, 2, wxALL, 10);
    h_sizer->Add(btn_panel, 0, wxALL, 10);
    content_panel_->SetSizer(content_sizer_);
    content_sizer_->Add(tb_name_panel);
    content_sizer_->Add(h_panel);
}

void wxCreateTableDialog::construct_grid() {
    // Remove existing grid if it exists
    if (grid_->GetNumberRows() > 0) {
        grid_->DeleteRows(0, grid_->GetNumberRows());
    }
    if (grid_->GetNumberCols() > 0) {
        grid_->DeleteCols(0, grid_->GetNumberCols());
    }

    grid_->AppendRows(default_row_num_);
    grid_->AppendCols(3);

    // grid_->HideRowLabels();
    grid_->SetRowLabelSize(10);
    grid_->SetColLabelValue(0, "name");
    grid_->SetColLabelValue(1, "type");
    grid_->SetColLabelValue(2, "constraints");

    grid_->SetColSize(0, 160);
    grid_->SetColSize(1, 160);
    grid_->SetColSize(2, 300);
    grid_->DisableCellEditControl();

    grid_->DisableDragRowSize();
    grid_->DisableDragColSize();


    wxArrayString type_choices;
    type_choices.Add("TEXT");
    type_choices.Add("INTEGER");
    type_choices.Add("FLOAT");
    type_choices.Add("BOOLEAN");
    type_choices.Add("DATE");
    type_choices.Add("DATETIME");
    type_choices.Add("YEAR");
    wxArrayString constraint_choices;
    constraint_choices.Add("NONE");
    constraint_choices.Add("PRIMARY KEY");
    constraint_choices.Add("NOT NULL");

    for (int row = 0; row < default_row_num_;++row) {
        grid_->SetCellEditor(row, 1, new wxGridCellChoiceEditor(type_choices));
        grid_->SetCellEditor(row, 2, new wxGridCellChoiceEditor(constraint_choices));
    }

    content_panel_->Layout();
}

ColumnDefinition wxCreateTableDialog::get_col_def_from_row(int row_index) {
    std::string name = grid_->GetCellValue(row_index, 0).ToStdString(); 
    std::string type = grid_->GetCellValue(row_index, 1).ToStdString(); 
    std::string constraints = grid_->GetCellValue(row_index, 2).ToStdString(); 
    return ColumnDefinition(name, type, constraints);
}

void wxCreateTableDialog::on_ok(wxCommandEvent& event) {

}

void wxCreateTableDialog::on_cancel(wxCommandEvent& event) {

}
