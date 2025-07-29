#include <assert.h>
#include "wx_table_schema_dialog.h"
#include "wx/wx.h"
#include "wx/grid.h"
#include "gui/gui_components/horizontal_panel.h"

wxTableSchemaDialog::wxTableSchemaDialog(wxWindow* parent) 
    : wxBaseDialog(parent, "Table Schema") 
{
    // INIT STUFF
    DatabaseManager& db_manager = DatabaseManager::getInstance();
    tb_manager_ = db_manager.get_tb_manager();

    // GUI
    HorizontalPanel* current_table_panel = new HorizontalPanel(content_panel_);
    wxStaticText* st = new wxStaticText(current_table_panel, wxID_ANY, "Current Table:");
    table_selection_box_ = new wxChoice(current_table_panel, wxID_ANY);
    wxButton* rename_btn = new wxButton(current_table_panel, wxID_ANY, "Rename");
    current_table_panel->add_children(st, table_selection_box_, rename_btn);

    grid_panel_ = new wxTableSchemaGridPanel(this, tb_manager_);

    // table selection box
    auto table_names = tb_manager_->get_table_names();
    for (const auto& name : table_names) {
        table_selection_box_->Append(name);
    }

    content_panel_->SetSizer(content_sizer_);
    content_sizer_->Add(current_table_panel, 0, wxALL, 10);
    content_sizer_->Add(grid_panel_, 1, wxEXPAND | wxALL, 10);

    content_panel_->Layout();

    // BIND
    table_selection_box_->Bind(wxEVT_CHOICE, &wxTableSchemaDialog::on_select_table, this);
    rename_btn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        wxString current = table_selection_box_->GetStringSelection();
        wxTextEntryDialog dlg(this, "Rename Table to:", "Rename Table", current);
        if (dlg.ShowModal() == wxID_OK) {
            wxString new_name = dlg.GetValue();
            tb_manager_->rename_table(current.ToStdString(), new_name.ToStdString());
            int sel_index = table_selection_box_->GetSelection();
            assert(sel_index != wxNOT_FOUND);
            table_selection_box_->Delete(sel_index);
            table_selection_box_->Insert(new_name, sel_index);
            table_selection_box_->SetSelection(sel_index);
        }
    });
}

void wxTableSchemaDialog::on_select_table(wxCommandEvent& event) {
    std::string selected_table_name = table_selection_box_->GetStringSelection().ToStdString();
    tb_manager_->set_current_table_name(selected_table_name);
    grid_panel_->init_grid_cols();
    grid_panel_->update_grid();
}
void wxTableSchemaDialog::on_rename(wxCommandEvent& event) {
    // change table_selection_box_ to type could get input
}
ColumnDefinition wxTableSchemaDialog::get_col_def_from_row(int row_index) {
    std::string name = grid_panel_->grid_->GetCellValue(row_index, 0).ToStdString(); 
    std::string type = grid_panel_->grid_->GetCellValue(row_index, 1).ToStdString(); 
    std::string constraints = grid_panel_->grid_->GetCellValue(row_index, 2).ToStdString(); 
    return ColumnDefinition(name, type, constraints);
}