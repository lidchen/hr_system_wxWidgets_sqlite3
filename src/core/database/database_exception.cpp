#include "database_exception.h"

DatabaseException::DatabaseException(const std::string err_msg)
   : std::runtime_error("[DB ERROR] " + err_msg) {}