#ifndef WX_CREATE_TABLE_DIALOG_H_
#define WX_CREATE_TABLE_DIALOG_H_

#include "wx/wx.h"
#include "wx/grid.h"
#include "core/database/schema/column_definition.h"
#include "gui/dialog/wx_base_dialog.h"
#include "gui/gui_components/grid_panel/wx_table_schema_grid_panel.h"
#include "gui/gui_components/labeled_text_ctrl.h"

class wxCreateTableDialog : public wxBaseDialog {
public:
    wxCreateTableDialog(wxWindow* parent);
private:
    TableSchema generate_schema();
    LabeledTextCtrl* tb_name_panel_;
    DatabaseTableManager* tb_manager_;
    wxTableSchemaGridPanel* grid_panel_;
    void on_ok(wxCommandEvent& event) override;
    void on_cancel(wxCommandEvent& event) override;
};

#endif // WX_CREATE_TABLE_DIALOG_H_