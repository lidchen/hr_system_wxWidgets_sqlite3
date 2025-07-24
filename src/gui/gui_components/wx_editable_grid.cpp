#include "wx_editable_grid.h"
#include <core/database/database_exception.h>

wxEditableGrid::wxEditableGrid(wxWindow* parent) 
    : wxGrid(parent, wxID_ANY)
{
    construct_grid();

    actual_rows_ = GetNumberRows();
    // Ensure at least 10 rows are visible
    if (GetNumberRows() < minimal_rows_showed) {
        AppendRows(minimal_rows_showed - GetNumberRows());
    }

    Bind(wxEVT_GRID_CELL_CHANGED, &wxEditableGrid::on_cell_change, this);
    // Dont forget to bind add & delete btn to event function
}

void wxEditableGrid::init_grid_style() {
    SetRowLabelSize(10);
    DisableDragRowSize();
    DisableDragColSize();
}

void wxEditableGrid::remove_existing_grid() {
    if (GetNumberRows() > 0) {
        DeleteRows(0, GetNumberRows());
    }
    if (GetNumberCols() > 0) {
        DeleteCols(0, GetNumberCols());
    }
}

void wxEditableGrid::on_cell_change(wxGridEvent& event) {
    int row = event.GetRow();
    // if is editing new line, ignore cell commit
    if (row == new_row_index_) {
        return;
    }

    int col = event.GetCol();
    // event.GetString() get the old value before modified
    // so get the new value directly from the grid
    // , save old value for rollback
    wxString old_value = event.GetString();
    wxString new_value = GetCellValue(row, col);
    try {
        commit_cell_change(row, col, new_value.ToStdString());
    } catch (const DatabaseException& e) {
        wxMessageBox("Error updating database: " + wxString(e.what()));
        // TODO: set col value back
        SetCellValue(row, col, old_value);
    }
}

void wxEditableGrid::on_add_line(wxCommandEvent& event) {
    AppendRows(1);
    actual_rows_++;
    int last_actual_row_index = actual_rows_ - 1;

    new_row_index_ = last_actual_row_index;
assert(new_row_index_ != -1);
    SetGridCursor(last_actual_row_index, 0);
    SetFocus();
    SelectBlock(last_actual_row_index, 0, last_actual_row_index, 0);

    // DO OTHERS AT CHILD CLASS
}

void wxEditableGrid::on_delete_line(wxCommandEvent& event) {
    // Get selected rows
    wxArrayInt selectedRows = GetSelectedRows();
    if (selectedRows.IsEmpty()) {
        wxLogWarning("Please select a row to delete.");
        return;
    }

    // Highlight the selected row
    int row = selectedRows[0];
    SelectRow(row, true); // true to deselect others
    SetGridCursor(row, 0);
    SetFocus();
    // Only allow deleting one row at a time for safety
    std::string col_name = GetCellValue(row, 0).ToStdString();

    // Confirm deletion
    int res = wxMessageBox(
        wxString::Format("Are you sure you want to delete column '%s'?", col_name),
        "Confirm Delete", wxYES_NO | wxICON_QUESTION, this);
    if (res != wxYES)
        return;

    try {
        commit_delete_line(row);
        construct_grid();
    } catch (DatabaseException& e) {
        wxLogError("Failed to delete column: %s", e.what());
    }

}