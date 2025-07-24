#include <assert.h>
#include "view_panel.h"
#include "core/database/database_stmt_builder.h"

ViewPanel::ViewPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY) 
{
    // DONT TOUCH THIS, THIS INIT STUFF
    grid_ = new wxGrid(this, wxID_ANY); 
    set_database();
    construct_grid();
    sizer_ = new wxBoxSizer(wxVERTICAL);
    sizer_->Add(grid_, 1, wxEXPAND | wxALL, 5); 
    auto schema = tb_manager_->get_current_table_schema();
    SetSizer(sizer_);
    // sizer_->SetSizeHints(this); 
    Layout();
    Fit(); 
    
    grid_->Bind(wxEVT_GRID_CELL_CHANGED, &ViewPanel::on_cell_changed, this);
}

void ViewPanel::set_database() {
    db_ = db_manager_.get_current_database();
    assert(db_->is_connected());
    tb_manager_ = std::make_unique<DatabaseTableManager>(db_);

    // helper local variables
    schema_ = tb_manager_->get_current_table_schema();
    table_name_ = tb_manager_->get_current_table_name();
    col_names_ = schema_.get_col_names();
    pk_name_ = schema_.get_pk_name();
    pk_index_ = schema_.get_pk_index();

assert(pk_index_ != -1 
    && (schema_.get_col_names()[pk_index_] == pk_name_));
}

void ViewPanel::construct_grid() {
    if (grid_) {
        grid_->ClearGrid();
    }
    // SET ROW
    std::string sql = "SELECT * FROM " + tb_manager_->get_current_table_name();
    Database::QueryResult q_result = db_->fetch_all(sql);
    if (q_result.row_num_ == 0) {
        // Empty table, set row label with 10 empty row
        auto schema = tb_manager_->get_current_table_schema();
        auto col_names = schema.get_col_names();
        int col_num = col_names.size();
        grid_->CreateGrid(10, col_num);
        grid_->SetColLabelSize(50);
        for (int col = 0; col < col_num; ++col) {
            grid_->SetColLabelValue(col, col_names[col]);
        }
    } else {
        // Construct table using QueryResult
        const int row_num = q_result.row_num_; 
        const int col_num = q_result.col_num_;
        if (row_num == 0) {
            grid_->CreateGrid(10, col_num);
        } else {
            grid_->CreateGrid(row_num, col_num);
        }
        grid_->SetRowLabelSize(25);
        grid_->SetColLabelSize(25);
        grid_->SetRowLabelAlignment(wxALIGN_RIGHT, wxALIGN_CENTER);
        grid_->SetLabelFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        for (int i = 0; i < row_num; ++i) {
            grid_->SetRowSize(i, 25);
        }

        // Set column labels
        for (int col = 0; col < col_num; ++col) {
            grid_->SetColLabelValue(col, q_result.column_names_[col]);
        }

        // Set cell values
        for (int row = 0; row < row_num; ++row) {
            for (int col = 0; col < col_num; ++col) {
                grid_->SetCellValue(row, col, q_result.row_values_[row][col]);
            }
        }
    }
}

void ViewPanel::on_cell_changed(wxGridEvent& event) {
    int row = event.GetRow();
    int col = event.GetCol();
    // wxEVT_GRID_CELL_CHANGED is fired after the cell value has changed,
    // so get the new value directly from the grid
    wxString old_value = event.GetString();
    wxString new_value = grid_->GetCellValue(row, col);
    try {
        commit_single_cell(row, col, new_value.ToStdString());
    } catch (const DatabaseException& e) {
        wxMessageBox("Error updating database: " + wxString(e.what()));
        // TODO: set col value back
        grid_->SetCellValue(row, col, old_value);
    }
}

std::string ViewPanel::get_pk_value(int row) {
    // Get the value of the primary key in the given row
    wxString pk_value = grid_->GetCellValue(row, pk_index_);
    return pk_value.ToStdString();
}

void ViewPanel::commit_single_cell(int row, int col, const std::string new_value) {
    const std::string& col_name = col_names_[col];
    const std::string& pk_value = get_pk_value(row);
    DatabaseStmtBuilder builder;
    std::string sql = builder.update(table_name_)
        .set(col_name, new_value)
        .where()
        .column(pk_name_).equals(pk_value)
        .build_with_and();
    std::cout << sql << "\n";
    db_->execute_sql(sql); 
}