#ifndef WX_EDITABLE_GRID_PANEL_H_
#define WX_EDITABLE_GRID_PANEL_H_

#include "wx/wx.h"
#include "wx/grid.h"

class wxEditableGridPanel : public wxPanel {
public:
    wxEditableGridPanel(wxWindow* parent);
    void init_state();
    void set_btn_status();
    wxPanel *grid_panel_;
    wxPanel* btn_pannel_;
    wxGrid* grid_;
    wxButton* add_btn_;
    wxButton* delete_btn_;
protected:
    wxBoxSizer* h_sizer_;
    virtual void init_grid_cols() = 0;
    virtual void update_grid() = 0;
    virtual void append_new_row() = 0;
    virtual void commit_row(int row) = 0;
    virtual void commit_row_delete(int row) = 0;
    virtual void commit_cell(int row, int col, const std::string& new_value) = 0;

    // I realize this feature conflict with my design
    // const int min_rows_showed = 10;
    int actual_rows_;
    int new_row_index_;
    // For handle pk change
    std::string prev_cell_value;
    void on_cell_change(wxGridEvent &event);
    void on_add_or_commit_row(wxCommandEvent& event);
    void on_delete_row(wxCommandEvent& event);
    // For convenience
    void remove_existing_grid_rows();
    void remove_existing_grid_cols();
};

#endif // WX_EDITABLE_GRID_PANEL_H_