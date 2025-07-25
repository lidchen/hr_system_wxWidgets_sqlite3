#ifndef WX_EDITABLE_GRID_H_
#define WX_EDITABLE_GRID_H_

#include "wx/wx.h"
#include "wx/grid.h"

// Show minimal lines of default row num
// provide interface for row changes' management (add, delete, cell change)

class wxEditableGrid : public wxGrid {
public:
    wxEditableGrid(wxWindow* parent);
    virtual void construct_grid() = 0;
    void init_grid_style();
    void remove_existing_grid();
    int actual_rows_;
private:
    bool is_editing_new_line() { return (new_row_index_ != -1); }
    int new_row_index_ = -1; // -1 -> not editing new line

    const int minimal_rows_showed = 10;
    void on_cell_change(wxGridEvent& event);
    void on_add_line(wxCommandEvent& event);
    void on_delete_line(wxCommandEvent& event);
    virtual void commit_cell_change(int row, int col, const std::string& new_value);
    virtual void commit_new_line(int row);
    virtual void commit_delete_line(int row);
};

#endif // WX_EDITABLE_GRID_H_
