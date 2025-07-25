#include "wx_table_schema_grid_panel.h"
#include "core/database/database_manager.h"

wxTableSchemaGridPanel::wxTableSchemaGridPanel(wxWindow* parent, DatabaseTableManager* tb_manager)
    : tb_manager_(tb_manager) 
    , wxEditableGridPanel(parent)
{
    type_choices.Add("TEXT");
    type_choices.Add("INTEGER");
    type_choices.Add("FLOAT");
    type_choices.Add("BOOLEAN");
    type_choices.Add("DATE");
    type_choices.Add("DATETIME");
    type_choices.Add("YEAR");
    constraint_choices.Add("NONE");
    constraint_choices.Add("PRIMARY KEY");
    constraint_choices.Add("NOT NULL");

    init_grid_cols();
    update_grid();

    Layout();
}
void wxTableSchemaGridPanel::init_grid_cols() {
    init_state();
    remove_existing_grid_cols();
    remove_existing_grid_rows();
    grid_->CreateGrid(0, 3);
    int row = tb_manager_->get_current_table_schema().get_row_num();
    grid_->AppendRows(row);
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
}

void wxTableSchemaGridPanel::update_grid() {
    remove_existing_grid_rows();
    TableSchema schema = tb_manager_->get_current_table_schema();
    int row = schema.get_row_num();
    grid_->AppendRows(row);

    int current_row = 0;
    for (const ColumnDefinition& col_def : schema.col_defs_) {
        grid_->SetCellValue(wxGridCellCoords(current_row, 0), col_def.name_); 
        grid_->SetCellValue(wxGridCellCoords(current_row, 1), col_def.type_to_string()); 
        grid_->SetCellValue(wxGridCellCoords(current_row, 2), col_def.constraints_to_string()); 
        current_row++;
    }
    actual_rows_ = grid_->GetNumberRows();

    for (int row = 0; row < actual_rows_;++row) {
        for (int col = 0; col < 3; ++col) {
            grid_->SetReadOnly(row, col, true);
        }
    }
    // // Ensure at least 10 rows are visible
    // if (actual_rows_ < min_rows_showed) {
    //     grid_->AppendRows(min_rows_showed - actual_rows_);
    // }
}

void wxTableSchemaGridPanel::append_new_row() {
    // do change actual_rows
    actual_rows_++;
    int last_row_index = actual_rows_ - 1;
    grid_->AppendRows(1);
    grid_->EnableCellEditControl(false);
    grid_->SetReadOnly(last_row_index, 0, false);
    grid_->SetReadOnly(last_row_index, 1, false);
    grid_->SetReadOnly(last_row_index, 2, false);
    grid_->SetCellEditor(last_row_index, 0, new wxGridCellTextEditor());
    grid_->SetCellEditor(last_row_index, 1, new wxGridCellChoiceEditor(type_choices));
    grid_->SetCellEditor(last_row_index, 2, new wxGridCellChoiceEditor(constraint_choices));
}
void wxTableSchemaGridPanel::commit_cell(int row, int col, const std::string& new_value) {
    // NO, YOU CANT CHANGE COLUMN IN SQLITE
}

void wxTableSchemaGridPanel::commit_row(int row) {
    try {
        tb_manager_->add_column(get_col_def_from_row(row));

        int last_row_index = actual_rows_ - 1;
        grid_->EnableCellEditControl(true);
        grid_->SetReadOnly(last_row_index, 0, true);
        grid_->SetReadOnly(last_row_index, 1, true);
        grid_->SetReadOnly(last_row_index, 2, true);
    } catch (DatabaseException e) {
        throw (e);
    }
}

void wxTableSchemaGridPanel::commit_row_delete(int row) {
    // Get selected rows
    // This could for mult row deletion
    wxArrayInt selectedRows = grid_->GetSelectedRows();
    if (selectedRows.IsEmpty()) {
        wxLogWarning("Please select a row to delete.");
        return;
    }
    // Highlight the selected row
    // int first_selected_row = selectedRows[0];
    // grid_->SelectRow(first_selected_row, true); // true to deselect others
    // grid_->SetGridCursor(first_selected_row, 0);
    // grid_->SetFocus();
    // Only allow deleting one row at a time for safety
    std::string col_name = grid_->GetCellValue(row, 0).ToStdString();
    try {
        tb_manager_->drop_column(col_name);
        actual_rows_--;
        update_grid();
    } catch (DatabaseException& e) {
        wxLogError("Failed to delete column: %s", e.what());
    }
}

ColumnDefinition wxTableSchemaGridPanel::get_col_def_from_row(int row_index) {
    std::string name = grid_->GetCellValue(row_index, 0).ToStdString(); 
    std::string type = grid_->GetCellValue(row_index, 1).ToStdString(); 
    std::string constraints = grid_->GetCellValue(row_index, 2).ToStdString(); 
    return ColumnDefinition(name, type, constraints);
}