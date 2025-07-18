#include "database.h"
#include "database_exception.h"
#include "wx/filename.h"
#include <iostream>

Database::Database(const wxString& db_path) 
    : DatabaseOpsBase(nullptr)
{
    // For database manager
    // check if db is vaild then close it
    int rc = sqlite3_open(db_path.c_str(), &db_);
    if (!db_ || rc != SQLITE_OK) {
        std::string err_msg = db_ ? sqlite3_errmsg(db_) : "Failed to create database handle";
        if (db_) sqlite3_close(db_);
        throw DatabaseException(err_msg);
    }
    // Extract filename from db_path (remove directory part)
    size_t pos = db_path.find_last_of(wxFileName::GetPathSeparator());
    filename_ = (pos != std::string::npos) ? db_path.substr(pos + 1) : db_path;
    sqlite3_close(db_);
    // transaction_manager_ = std::make_unique<TransactionManager>(db_);
    // file_handler_ = std::make_unique<DatabaseFileHandler>(db_);
}
Database::~Database() {
    if (db_) {
        sqlite3_close(db_);
    }
}
bool Database::is_opened() {
    return db_ ? true : false;
}
void Database::close() {
    if (db_) {
        sqlite3_close(db_);
    }
}
void Database::import_from_csv(const std::string& filepath) {
}
void Database::begin_transaction() {
    transaction_manager_->begin();
    std::cout << "Transcation begin\n";
}
void Database::commit_transaction() {
    transaction_manager_->commit();
    std::cout << "Commit\n";
}
void Database::rollback_transaction() {
    transaction_manager_->rollback();
    std::cout << "Rollback\n";
}
void Database::debug_state() const {
    std::cout << "Database state check:" << std::endl;
    std::cout << "  db_ pointer: " << (void*)db_ << std::endl;
    if (db_) {
        std::cout << "  Last error: " << sqlite3_errmsg(db_) << std::endl;
        std::cout << "  Extended error: " << sqlite3_extended_errcode(db_) << std::endl;
    }
}