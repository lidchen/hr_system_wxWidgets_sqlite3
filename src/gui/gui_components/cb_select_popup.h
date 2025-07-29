#ifndef CB_SELECT_POPUP
#define CB_SELECT_POPUP

#include <vector>
#include "wx/wx.h"
#include "wx/popupwin.h"

class CbSelectPopup : public wxPopupTransientWindow {
public:
    CbSelectPopup(wxWindow* parent, std::vector<std::string> cb_names);
    std::vector<bool> get_selection();
private:
    std::vector<wxCheckBox*> cb_container_;
};

#endif // CB_SELECT_POPUP
