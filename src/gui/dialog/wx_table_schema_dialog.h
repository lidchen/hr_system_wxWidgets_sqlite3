#ifndef WX_TABLE_SCHEMA_DIALOG_H_
#define WX_TABLE_SCHEMA_DIALOG_H_

#include "wx/wx.h"
#include "wx/grid.h"

#include "core/database/database_manager.h"
#include "core/database/database_table_manager.h"
#include "gui/gui_components/listbox.h"
#include "gui/gui_components/grid_panel/wx_table_schema_grid_panel.h"
#include "gui/dialog/wx_base_dialog.h"

class wxTableSchemaDialog : public wxBaseDialog{
public:
    wxTableSchemaDialog(wxWindow* parent);
    ColumnDefinition get_col_def_from_row(int row_index);
    ~wxTableSchemaDialog() = default;

private:
    void on_rename(wxCommandEvent& event);
    void on_select_table(wxCommandEvent& event);

    DatabaseTableManager* tb_manager_;
    wxChoice* table_selection_box_;
    wxTableSchemaGridPanel* grid_panel_;
};

#endif // WX_TABLE_SCHEMA_DIALOG_H_