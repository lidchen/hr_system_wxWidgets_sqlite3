#include "wx_create_table_dialog.h"
#include "gui/gui_components/horizontal_panel.h"
#include "gui/gui_components/labeled_text_ctrl.h"
#include "core/database/database_manager.h"

wxCreateTableDialog::wxCreateTableDialog(wxWindow* parent)
    : wxBaseDialog(parent, "Create Table")
{
    // INIT STUFF
    DatabaseManager& db_mananger = DatabaseManager::getInstance();
    tb_manager_ = db_mananger.get_tb_manager();

    // GUI
    tb_name_panel_ = new LabeledTextCtrl(content_panel_, wxID_ANY, "new table name: ", "", wxDefaultPosition, wxDefaultSize);
    grid_panel_ = new wxTableSchemaGridPanel(this, tb_manager_, TableSchemaGridMode::CREATE_TABLE);

    content_panel_->SetSizer(content_sizer_);
    content_sizer_->Add(tb_name_panel_, 0, wxALL, 10);
    content_sizer_->Add(grid_panel_, 1, wxEXPAND | wxALL, 10);

    content_panel_->Layout();

    Bind(wxEVT_BUTTON, &wxCreateTableDialog::on_ok, this, wxID_OK);
}
TableSchema wxCreateTableDialog::generate_schema() {
    std::string tb_name = tb_name_panel_->get_value().ToStdString();
    // Trim whitespace from table name
    tb_name.erase(0, tb_name.find_first_not_of(" \t\n\r\f\v"));
    tb_name.erase(tb_name.find_last_not_of(" \t\n\r\f\v") + 1);
    if(tb_name.empty()) {
        throw DatabaseException("Table name cannot be empty or only spaces.");
    }
    auto col_defs = grid_panel_->generate_col_defs();
    TableSchema schema(tb_name, col_defs);
    if(schema.is_empty()) {
        throw DatabaseException("Table is empty");
    }
    if(!schema.validate_contains_pk()) {
        throw DatabaseException("Table should contain a primary key.");
    }
    return TableSchema(tb_name, col_defs);
}
void wxCreateTableDialog::on_ok(wxCommandEvent& event) {
    try {
        tb_manager_->create_table(generate_schema());
        EndModal(wxID_OK);
    } catch(const DatabaseException& e) {
        wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR, this);
    }
}
void wxCreateTableDialog::on_cancel(wxCommandEvent& event) {

}
