#ifndef WX_TABLE_SCHEMA_DIALOG_H_
#define WX_TABLE_SCHEMA_DIALOG_H_

#include "gui/dialog/wx_base_dialog.h"
#include "gui/gui_components/listbox.h"
#include "core/database/database_manager.h"
#include "core/database/database_table_manager.h"

class wxTableSchemaDialog : public wxBaseDialog{
private:
    std::unique_ptr<DatabaseTableManager> tb_manager_;
    void list_schemas();
public:
    wxTableSchemaDialog(wxWindow* parent);
    ~wxTableSchemaDialog() = default;
};


#endif // WX_TABLE_SCHEMA_DIALOG_H_