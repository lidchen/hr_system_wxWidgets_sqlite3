#ifndef DATABASE_STMT_BUILDER_EXCEPTION_H_
#define DATABASE_STMT_BUILDER_EXCEPTION_H_

#include <stdexcept>
#include <string>
#include "core/database/database_exception.h"

class DatabaseStmtBuilderException : public DatabaseException {
public:
    explicit DatabaseStmtBuilderException(const std::string err_msg);
};

#endif // DATABASE_STMT_BUILDER_EXCEPTION_H_
