#ifndef WX_CREATE_TABLE_DIALOG_H_
#define WX_CREATE_TABLE_DIALOG_H_

#include "wx/wx.h"
#include "wx/grid.h"
#include "core/database/schema/column_definition.h"
#include "gui/dialog/wx_base_dialog.h"
#include "core/database/database_manager.h"
#include "core/database/database_table_manager.h"

class wxCreateTableDialog : public wxBaseDialog {
public:
    wxCreateTableDialog(wxWindow* parent);
private:
    const int default_row_num_ = 10; // Set default blank row num when open dialog
    std::unique_ptr<DatabaseTableManager> tb_manager_;
    wxGrid* grid_;
    // void on_add(wxCommandEvent& event);
    // void on_alter(wxCommandEvent& event);
    ColumnDefinition get_col_def_from_row(int row_index);
    void construct_grid();
    void on_ok(wxCommandEvent& event) override;
    void on_cancel(wxCommandEvent& event) override;
};

#endif // WX_CREATE_TABLE_DIALOG_H_