#include "wx_table_schema_dialog.h"
#include "wx/wx.h"
#include "wx/grid.h"
#include "gui/gui_components/horizontal_panel.h"

wxTableSchemaDialog::wxTableSchemaDialog(wxWindow* parent) 
    : wxBaseDialog(parent, "Table Schema") 
{
    DatabaseManager& db_manager = DatabaseManager::getInstance();
    tb_manager_ = std::make_unique<DatabaseTableManager>(db_manager.get_current_database());

    HorizontalPanel* current_table_panel = new HorizontalPanel(content_panel_);
    wxStaticText* st = new wxStaticText(current_table_panel, wxID_ANY, "Current Table:");
    table_selection_box = new wxChoice(current_table_panel, wxID_ANY);
    auto table_names = tb_manager_->get_table_names();
    for (const auto& name : table_names) {
        table_selection_box->Append(name);
    }
    current_table_panel->add_children(st, table_selection_box);

    wxPanel* h_panel = new wxPanel(content_panel_, wxID_ANY);
    wxPanel* btn_panel = new wxPanel(h_panel, wxID_ANY);
    wxBoxSizer* grid_sizer = new wxBoxSizer(wxHORIZONTAL);
    grid_ = new wxGrid(h_panel, wxID_ANY);
    grid_sizer->Add(grid_, 1, wxEXPAND);

    content_sizer_->Add(current_table_panel);
    content_sizer_->Add(h_panel);

    grid_->CreateGrid(0, 3);
    construct_grid();

    wxBoxSizer* h_sizer = new wxBoxSizer(wxHORIZONTAL);
    h_sizer->Add(grid_, 2, wxALL, 10);
    h_sizer->Add(btn_panel, 0, wxALL, 10);

    add_btn = new wxButton(btn_panel, wxID_ANY, "add");
    wxButton* alter_btn = new wxButton(btn_panel, wxID_ANY, "alter");
    wxButton* delete_btn = new wxButton(btn_panel, wxID_ANY, "delete");
    wxBoxSizer* btn_sizer = new wxBoxSizer(wxVERTICAL);
    btn_sizer->Add(add_btn, 0, wxALL, 10);
    btn_sizer->Add(alter_btn, 0, wxALL, 10);
    btn_sizer->Add(delete_btn, 0, wxALL, 10);

    btn_panel->SetSizer(btn_sizer);
    h_panel->SetSizer(h_sizer);
    content_panel_->SetSizer(content_sizer_);
    content_panel_->Layout();

    list_schemas();
    table_selection_box->Bind(wxEVT_CHOICE, &wxTableSchemaDialog::on_select_table, this);
    add_btn->Bind(wxEVT_BUTTON, &wxTableSchemaDialog::on_add, this);
    delete_btn->Bind(wxEVT_BUTTON, &wxTableSchemaDialog::on_drop_column, this);
}

void wxTableSchemaDialog::construct_grid() {
    // Remove existing grid if it exists
    if (grid_->GetNumberRows() > 0) {
        grid_->DeleteRows(0, grid_->GetNumberRows());
    }
    if (grid_->GetNumberCols() > 0) {
        grid_->DeleteCols(0, grid_->GetNumberCols());
    }
    row_num_ = tb_manager_->get_current_table_schema().get_row_num();
    grid_->AppendRows(row_num_);
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

    for (int row = 0; row < row_num_;++row) {
        for (int col = 0; col < 3; ++col) {
            grid_->SetReadOnly(row, col, true);
        }
    }
    content_panel_->Layout();
}

// Call construct_grid() before this
void wxTableSchemaDialog::list_schemas() {
    TableSchema schema = tb_manager_->get_current_table_schema();
    int current_row = 0;
    for (const ColumnDefinition& col_def : schema.col_defs_) {
        grid_->SetCellValue(wxGridCellCoords(current_row, 0), col_def.name_); 
        grid_->SetCellValue(wxGridCellCoords(current_row, 1), col_def.type_to_string()); 
        grid_->SetCellValue(wxGridCellCoords(current_row, 2), col_def.constraints_to_string()); 
        current_row++;
    }
}
void wxTableSchemaDialog::append_to_grid(const ColumnDefinition& col_def) {
    grid_->SetCellValue(wxGridCellCoords(row_num_, 0), col_def.name_); 
    grid_->SetCellValue(wxGridCellCoords(row_num_, 1), col_def.type_to_string()); 
    grid_->SetCellValue(wxGridCellCoords(row_num_, 2), col_def.constraints_to_string()); 
    row_num_++;
}
void wxTableSchemaDialog::on_add(wxCommandEvent& event) {
    // Append a new row to the grid
    grid_->AppendRows(1);
    row_num_++;
    int last_row_index = row_num_ - 1;

    grid_->EnableCellEditControl(true);
    grid_->SetReadOnly(last_row_index, 0, false);
    grid_->SetReadOnly(last_row_index, 1, false);
    grid_->SetReadOnly(last_row_index, 2, false);

    grid_->SetGridCursor(last_row_index, 0);
    grid_->SetFocus();
    grid_->SelectBlock(last_row_index, 0, last_row_index, 0);

    // grid_->SetCellBackgroundColour(last_row_index, 0, wxColour(0, 255, 0));
    // Column 0: TextCtrl editor (default)
    grid_->SetCellEditor(last_row_index, 0, new wxGridCellTextEditor());

    // Column 1: Choice editor for type
    wxArrayString type_choices;
    type_choices.Add("TEXT");
    type_choices.Add("INTEGER");
    type_choices.Add("FLOAT");
    type_choices.Add("DATE");
    type_choices.Add("BOOLEAN");
    grid_->SetCellEditor(last_row_index, 1, new wxGridCellChoiceEditor(type_choices));

    // Column 2: Choice editor for constraints
    wxArrayString constraint_choices;
    constraint_choices.Add("NONE");
    constraint_choices.Add("PRIMARY KEY");
    constraint_choices.Add("NOT NULL");
    grid_->SetCellEditor(last_row_index, 2, new wxGridCellChoiceEditor(constraint_choices));

    content_panel_->Layout();

    // Change add button to ok
    add_btn->SetLabel("ok");
    add_btn->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
    add_btn->SetForegroundColour(*wxWHITE);
    add_btn->Bind(wxEVT_BUTTON, &wxTableSchemaDialog::on_commit_new_line, this);
}
void wxTableSchemaDialog::on_commit_new_line(wxCommandEvent& event) {
    int last_row_index = row_num_ - 1;
    try {
        tb_manager_->add_column(get_col_def_from_row(last_row_index));

        grid_->EnableCellEditControl(false);
        grid_->SetReadOnly(last_row_index, 0, true);
        grid_->SetReadOnly(last_row_index, 1, true);
        grid_->SetReadOnly(last_row_index, 2, true);
        add_btn->SetLabel("add");
        add_btn->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));
        add_btn->SetForegroundColour(*wxWHITE);
        add_btn->Bind(wxEVT_BUTTON, &wxTableSchemaDialog::on_add, this);
    } catch (DatabaseException e) {
        wxLogError("cant add new line: %s", e.what());
    }
}
void wxTableSchemaDialog::on_drop_column(wxCommandEvent& event) {
    // Get selected rows
    wxArrayInt selectedRows = grid_->GetSelectedRows();
    if (selectedRows.IsEmpty()) {
        wxLogWarning("Please select a row to delete.");
        return;
    }

    // Highlight the selected row
    int row = selectedRows[0];
    grid_->SelectRow(row, true); // true to deselect others
    grid_->SetGridCursor(row, 0);
    grid_->SetFocus();
    // Only allow deleting one row at a time for safety
    std::string col_name = grid_->GetCellValue(row, 0).ToStdString();

    // Confirm deletion
    int res = wxMessageBox(
        wxString::Format("Are you sure you want to delete column '%s'?", col_name),
        "Confirm Delete", wxYES_NO | wxICON_QUESTION, this);
    if (res != wxYES)
        return;

    try {
        tb_manager_->drop_column(col_name);
        row_num_--;
        construct_grid();
        list_schemas();
    } catch (DatabaseException& e) {
        wxLogError("Failed to delete column: %s", e.what());
    }
}

void wxTableSchemaDialog::on_select_table(wxCommandEvent& event) {
    std::string selected_table_name = table_selection_box->GetStringSelection().ToStdString();
    tb_manager_->set_current_table_name(selected_table_name);
    construct_grid();
    list_schemas();
}
ColumnDefinition wxTableSchemaDialog::get_col_def_from_row(int row_index) {
    std::string name = grid_->GetCellValue(row_index, 0).ToStdString(); 
    std::string type = grid_->GetCellValue(row_index, 1).ToStdString(); 
    std::string constraints = grid_->GetCellValue(row_index, 2).ToStdString(); 
    return ColumnDefinition(name, type, constraints);
}