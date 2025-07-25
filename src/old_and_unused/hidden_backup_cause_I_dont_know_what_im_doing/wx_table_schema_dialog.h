#ifndef WX_TABLE_SCHEMA_DIALOG_H_
#define WX_TABLE_SCHEMA_DIALOG_H_

#include "wx/wx.h"
#include "wx/grid.h"

#include "gui/dialog/wx_base_dialog.h"
#include "gui/gui_components/listbox.h"
#include "core/database/database_manager.h"
#include "core/database/database_table_manager.h"

class wxTableSchemaDialog : public wxBaseDialog{
private:
    std::unique_ptr<DatabaseTableManager> tb_manager_;
    wxChoice* table_selection_box;
    wxButton* add_btn;
    wxGrid* grid_;
    int row_num_;
public:
    wxTableSchemaDialog(wxWindow* parent);
    void construct_grid();
    void list_schemas();
    // void clear_grid();
    ColumnDefinition get_col_def_from_row(int row_index);
    void append_to_grid(const ColumnDefinition &col_defs);
    void on_add(wxCommandEvent &event);
    void enable_grid_hover_highlight();
    void on_grid_mouse_motion(wxMouseEvent &event);
    void on_select_table(wxCommandEvent &event);
    void on_commit_new_line(wxCommandEvent &event);
    void on_drop_column(wxCommandEvent &event);
    ~wxTableSchemaDialog() = default;
};

#endif // WX_TABLE_SCHEMA_DIALOG_H_