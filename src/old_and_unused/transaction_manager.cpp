#include "transaction_manager.h"

void TransactionManager::begin() {
    execute_sql("BEGIN TRANSACTION");
}

void TransactionManager::commit() {
    execute_sql("COMMIT");
}

void TransactionManager::rollback() {
    execute_sql("ROLLBACK");
}