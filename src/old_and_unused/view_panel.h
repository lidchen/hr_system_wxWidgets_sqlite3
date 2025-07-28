#ifndef VIEW_PANEL_H_
#define VIEW_PANEL_H_

#include "wx/wx.h"
#include "wx/grid.h"
#include "core/database/schema/table_schema.h"
#include "core/database/database.h"
#include "core/database/database_manager.h"
#include "core/database/database_table_manager.h"

class ViewPanel : public wxPanel {
public:
    ViewPanel(wxWindow *parent);
    void set_database();
    void construct_grid();

    // New: Get user input from the grid and transform to SQL
    // std::vector<std::map<std::string, wxString>> get_grid_data() const;
    // wxString generate_insert_sql(const wxString& table_name) const;

private:
    Database* db_;
    DatabaseManager& db_manager_ = DatabaseManager::getInstance();
    std::unique_ptr<DatabaseTableManager> tb_manager_;

    // Helper local variable
    // For preformance, I dont want to get this every update
    TableSchema schema_;
    std::vector<std::string> col_names_;
    std::string pk_name_;
    std::string table_name_;
    int pk_index_;


    // Grid
    wxGrid* grid_;
    wxSizer* sizer_;

    std::string get_pk_value(int row);
    void on_select_table(wxCommandEvent& event);
    void on_cell_changed(wxGridEvent& event);
    void commit_single_cell(int row, int col, const std::string new_value);
};

#endif // VIEW_PANEL_H_