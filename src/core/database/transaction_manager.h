#ifndef TRANSACTION_MANAGER_H_
#define TRANSACTION_MANAGER_H_

#include "sqlite3.h"
#include "database_exception.h"
#include "database_ops_base.h"

class TransactionManager : public DatabaseOpsBase {
public:
    TransactionManager(sqlite3* db) : DatabaseOpsBase(db) {}
    TransactionManager(const TransactionManager& other) = delete;
    TransactionManager& operator= (const TransactionManager& other) = delete;
    void begin();
    void commit();
    void rollback();
};

#endif // TRANSACTION_MANAGER_H_