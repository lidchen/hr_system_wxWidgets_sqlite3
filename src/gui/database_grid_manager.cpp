#include "database_grid_manager.h"
#include "wx/font.h"

DatabaseGridManager::DatabaseGridManager(wxGrid* grid)
    : wx_grid_(grid)
{
// temp, for debug
// construct_by_result(db->fetch_all("SELECT * FROM employee"));
}
void DatabaseGridManager::construct_by_result(const DatabaseOpsBase::QueryResult& q_result) {
    clear_grid(); 
    const int row_num = q_result.row_num_;    
    const int col_num = q_result.col_num_;    
    wx_grid_->CreateGrid(row_num, col_num);
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
