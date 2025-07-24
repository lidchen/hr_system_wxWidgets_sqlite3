#ifndef WX_IMPORT_DIALOG_H_
#define WX_IMPORT_DIALOG_H_

#include "wx/wx.h"
#include "core/database/database_manager.h"
#include "core/database/database_exception.h"
#include "gui/dialog/wx_base_dialog.h"
#include "gui/view_panel.h"

class wxImportDialog : public wxBaseDialog {
public:
    wxImportDialog(wxWindow* parent);
private:
    std::string file_path_;
    void get_file_path_from_user();
    void construct_preview_panel();
    ViewPanel* preview_panel_;
    DatabaseManager& db_manager = DatabaseManager::getInstance();
};

#endif // WX_IMPORT_DIALOG_H_
