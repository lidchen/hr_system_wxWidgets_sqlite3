#ifndef WX_GET_FOLDER_PATH_DIALOG
#define WX_GET_FOLDER_PATH_DIALOG

#include <functional>
#include "wx/wx.h"

class wxGetFolderPathDialog : wxDirDialog {
public:
    using Callback = std::function<void(const wxString&)>;
    wxGetFolderPathDialog(wxWindow* parent, Callback cb);
    int ShowModal() override;
    wxString get_path() const;
private:
    Callback callback_;
};

#endif // WX_GET_FOLDER_PATH_DIALOG