#include "wx_get_folder_path_dialog.h"
#include <wx/dirdlg.h>

wxGetFolderPathDialog::wxGetFolderPathDialog(wxWindow* parent, Callback cb)
    : wxDirDialog(parent, "Select Directory", "", wxDD_DEFAULT_STYLE), callback_(cb)
{
    // ShowModal();
}
int wxGetFolderPathDialog::ShowModal() {
    int result = wxDirDialog::ShowModal();
    if (result == wxID_OK && callback_) {
        callback_(GetPath());
    }
    return result;
}
wxString wxGetFolderPathDialog::get_path() const {
    return GetPath();
}
// std::string wxGetFolderPathDialog::on_set(wxCommandEvent& event) {
//     if (this->ShowModal() == wxID_OK) {
//         wxString path = this->GetPath();
//         return path.ToStdString();
//     }
//     return "";
// }