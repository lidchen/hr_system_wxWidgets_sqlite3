#include "database_grid_manager.h"
#include "wx/font.h"

DatabaseGridManager::DatabaseGridManager(wxGrid* grid)
    : wx_grid_(grid)
{
}

void DatabaseGridManager::construct_grid(const TableSchema& schema) {
    // for (const auto& col_def : schema.col_defs_) {

    // }
}

// TODO: This could only work of set values, cant be used for construct grid
void DatabaseGridManager::construct_by_result(const Database::QueryResult& q_result) {
    clear_grid(); 
    const int row_num = q_result.row_num_; 
    const int col_num = q_result.col_num_;
    if (row_num == 0) {
        wx_grid_->CreateGrid(10, col_num);
    } else {
        wx_grid_->CreateGrid(row_num, col_num);
    }
    wx_grid_->SetRowLabelSize(50);
    wx_grid_->SetColLabelSize(25);
    wx_grid_->SetRowLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
    wx_grid_->SetLabelFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    for (int i = 0; i < row_num; ++i) {
        wx_grid_->SetRowSize(i, 25);
    }

    // Set column labels
    for (int col = 0; col < col_num; ++col) {
        wx_grid_->SetColLabelValue(col, q_result.column_names_[col]);
    }

    // Set cell values
    for (int row = 0; row < row_num; ++row) {
        for (int col = 0; col < col_num; ++col) {
            wx_grid_->SetCellValue(row, col, q_result.row_values_[row][col]);
        }
    }
}
void DatabaseGridManager::clear_grid() {
    if (wx_grid_) {
        wx_grid_->ClearGrid();
        // if (wx_grid_->GetNumberRows() > 0)
        //     wx_grid_->DeleteRows(0, wx_grid_->GetNumberRows());
        // if (wx_grid_->GetNumberCols() > 0)
        //     wx_grid_->DeleteCols(0, wx_grid_->GetNumberCols());
    }
}
