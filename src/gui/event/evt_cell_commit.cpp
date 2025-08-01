#include "evt_cell_commit.h"

wxDEFINE_EVENT(EVT_CELL_COMMIT, EvtCellCommit);

EvtCellCommit::EvtCellCommit(int row, int col, const std::string& value)
    : wxCommandEvent(EVT_CELL_COMMIT)
    , row_(row), col_(col), value_(value) {}

EvtCellCommit::EvtCellCommit(int row, int col, const std::string& value, const std::string& prev_value)
    : wxCommandEvent(EVT_CELL_COMMIT)
    , row_(row), col_(col), value_(value), prev_value_(prev_value) {}


wxEvent* EvtCellCommit::Clone() const {
    return new EvtCellCommit(*this);
}

