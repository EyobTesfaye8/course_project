#include "sql.h"
#include <fstream>
#include <experimental/filesystem>

using namespace experimental;
void RecoredLog() {
    try {
        Connection* con = connectionEstablisher();
        Statement* stmt = con->createStatement();
        ResultSet* res = stmt->executeQuery("SHOW VARIABLES LIKE 'general_log_file'");

        string logPath;
        if (res->next()) {
            logPath = res->getString("Value");
        }
        else {
            cerr << "Could not retrieve general_log_file path." << endl;
        }

        ifstream infile("C:\\ProgramData\\MySQL\\MySQL Server 8.0\\Data\\MOLTEN.log");
        if (!infile.is_open()) {
            cerr << "Failed to open the general log file. Check MySQL permissions." << endl;
        }
        filesystem::create_directory("Logs");

        ofstream outfile("Logs/mysql_general_log.txt");
        if (!outfile.is_open()) {
            cerr << "Failed to create log file in Logs directory." << endl;
            return;
        }

        string line;
        while (getline(infile, line)) {
            outfile << line << endl;
        }

        delete res;
        delete stmt;
        delete con;
    }
    catch (sql::SQLException& e) {
        cout << "SQL Exception: " << e.what() << endl;
    }
}