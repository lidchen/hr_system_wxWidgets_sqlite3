#ifndef WELCOME_PANEL_H_
#define WELCOME_PANEL_H_

#include "wx/wx.h"

class WelcomePanel : public wxPanel {
public:
    WelcomePanel(wxWindow* parent);

private:
    void on_db_manager(wxCommandEvent& event);
    void on_quit(wxCommandEvent& event);
};

#endif // WELCOME_PANEL_H_