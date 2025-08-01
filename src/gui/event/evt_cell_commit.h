#ifndef EVT_CELL_COMMIT_H_
#define EVT_CELL_COMMIT_H_

#include <string>
#include "wx/wx.h"

class EvtCellCommit : public wxCommandEvent {
public:
    EvtCellCommit(int row, int col, const std::string& value);
    EvtCellCommit(int row, int col, const std::string& value, const std::string& prev_value);
    wxEvent* Clone() const override;
    int get_row() const {return row_;}
    int get_col() const {return col_;}
    std::string get_value() const {return value_;}
    std::string get_prev_value() const {return prev_value_;}
private:
    int row_;
    int col_;
    std::string value_;
    std::string prev_value_;
};

wxDECLARE_EVENT(EVT_CELL_COMMIT, EvtCellCommit);

#endif // EVT_CELL_COMMIT_H_