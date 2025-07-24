#include "hr_system.h"
#include "gui/main_frame.h"
#include "gui/welcome_panel.h"

IMPLEMENT_APP(MainApp);

bool MainApp::OnInit() {
    try{
        wxFrame* main_frame = new MainFrame("wxDB");
        main_frame->Show(true);
    } catch(DatabaseException e) {
        std::cerr << e.what() << "\n";
    }
    return true;
}