#include <iostream>
#include "core/database/database_manager.h"
#include "cli/cli_util.h"

// TEMP FOR TEST
const std::string insert_sql =  "INSERT INTO STUDENT (ID, FIRST_NAME, LAST_NAME, EMAIL) "
                                "VALUES(1, 'John', 'Doe', 'john@doe.com'),"
                                "(2, 'Peter', 'Griffin', 'peter@griffin.com'),"
                                "(3, 'Homer', 'Simpson', 'homer@simpson.com')";

const std::string create_table_sql = "CREATE TABLE STUDENT(" 
                                    "ID INT PRIMARY KEY NOT NULL,"
                                    "FIRST_NAME TEXT NOT NULL,"
                                    "LAST_NAME TEXT NOT NULL,"
                                    "EMAIL TEXT NOT NULL)";
int main(int, char**){
    std::string project_path = "/Users/lid/Library/CloudStorage/OneDrive-Personal/Develop/work/hm_system/";
    try {
        DatabaseStmtBuilder builder;
        DatabaseManager& db_manager_ = DatabaseManager::getInstance();
        db_manager_.set_database_dir(project_path + "resources/sql");
        db_manager_.create_database("big employee table");
        db_manager_.set_current_database("big employee table");
        Database* db = db_manager_.get_current_database();
        // db->begin_transaction();
        builder.create_table("EMPLOYEE")
            .add_column("ID INT PRIMARY KEY NOT NULL")
            .add_column("FIRST_NAME TEXT")
            .add_column("LAST_NAME TEXT")
            .add_column("SEX TEXT")
            .add_column("EMAIL TEXT")
            .add_column("PHONE TEXT")
            .add_column("BIRTH DATE")
            .add_column("TITLE TEXT");
        db->execute_sql(builder.build());
        db->import_data_from_csv("employee", project_path + "/resources/employee.csv");
        // builder.select("FIRST_NAME, TITLE")
        //     .from("employee")
        //     .where("FIRST_NAME LIKE 'Jea%'");
        // auto result = db.fetch_all(builder.build());
        // CLIUtil::print_query_result(result);
        // db.rollback_transaction();
    } catch (DatabaseException& e) {
        std::cout << e.what() << "\n";
    }
}
