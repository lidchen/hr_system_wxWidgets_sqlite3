#include "database_manager.h"
#include "database_exception.h"
#include <iostream>

Database::Database(const std::string& filename) 
    : DatabaseOpsBase(nullptr){
    int rc = sqlite3_open(filename.c_str(), &db_);
    if (rc != SQLITE_OK) {
        std::string err_msg = sqlite3_errmsg(db_);
        sqlite3_close(db_);
        throw DatabaseException(err_msg);
    }
    transaction_manager_ = std::make_unique<TransactionManager>(db_);
    file_handler_ = std::make_unique<DatabaseFileHandler>(db_);
}
Database::~Database() {
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