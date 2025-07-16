#ifndef DATABASE_EXCEPTION_H_
#define DATABASE_EXCEPTION_H_

#include <stdexcept>
#include <string>

class DatabaseException : public std::runtime_error {
public:
    explicit DatabaseException(const std::string err_msg);
};

#endif