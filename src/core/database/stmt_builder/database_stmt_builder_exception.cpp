#include "database_stmt_builder_exception.h"

DatabaseStmtBuilderException::DatabaseStmtBuilderException(const std::string err_msg) 
    : DatabaseException("[Database_stmt_builder error]: " + err_msg) {}