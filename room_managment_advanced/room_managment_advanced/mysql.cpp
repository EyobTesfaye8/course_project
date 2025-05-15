#include "sql.h"

Connection* connectionEstablisher() {
    try {
        MySQL_Driver* driver;
        Connection* con;

        driver = get_mysql_driver_instance();
        con = driver->connect("tcp://localhost:3306",
            "root", "12345678");

        con->setSchema("hotel_room_manager");
        return con;
    }
    catch (SQLException& e) {
        cout << "Error: " << e.what() << endl;
        return NULL;
    }
}

bool checkAdmin(string* username, string* password) {
    try {
        bool isAdmin = false;
        Connection* con = connectionEstablisher();
        Statement* stmt = con->createStatement();
        string admins = "select admin_username, admin_password from adminstrator";
        ResultSet* res = stmt->executeQuery(admins);
       
        while (res->next()) {
            if (res->getString("admin_username") == *username && res->getString("admin_password") == *password) {
                isAdmin = true;
                break;
            }
        }
        delete con;
        delete stmt;
        delete res;
        return isAdmin;
    }
    catch (SQLException& e) {
        cout << "Error: " << e.what() << endl;
        return false;
    }
}

bool roomStatusChecker(int room_number) {
    try {
        bool isBooked = false;
        Connection* con = connectionEstablisher();
        PreparedStatement* pstmt = con->prepareStatement("select full_name from customer where room_number = ?");
        pstmt->setInt(1, room_number);
        ResultSet* res = pstmt->executeQuery();
        
        if (res->next()) {
            isBooked = true;
        }
        return isBooked;
    }
    catch (SQLException& e) {
        cout << "Error: " << e.what() << endl;
        return false;
    }
}

void aboutRoom(int* room_number) {
    try {
        Connection* con = connectionEstablisher();
        PreparedStatement* pstmt = con->prepareStatement("select * from rooms where room_number = ?");
        pstmt->setInt(1, *room_number);
        ResultSet* res = pstmt->executeQuery();

        while (res->next()) {
            cout << "\t\t-------------------------------------------------------------\n";
            cout << "\t\t\tRoom details\n";
            cout << "\t\t\t------------\n";
            cout << "\t\t\tRoom number: \t" << res->getString("room_number") << endl;
            cout << "\t\t\tRoom type: \t" << res->getString("room_type") << endl;
            cout << "\t\t\tRoom price: \t" << res->getDouble("room_price") << endl;
            cout << "\t\t\tRoom floor: \t" << res->getString("room_floor") << endl;
            cout << "\t\t-------------------------------------------------------------\n";
        }
        delete pstmt;
        delete res;
        delete con;
    }
    catch (SQLException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

//customer

void reserveRoom(int* ID, string* name, int* phone_number, string* exit_date, char* Pmethod, int* room_number) {
    try {
        string payment_method;
        Connection* con = connectionEstablisher();
        if (!roomStatusChecker(*room_number)) {
            PreparedStatement* pstmt = con->prepareStatement("insert into customer(customer_id, full_name, phone_number, exit_date, payment_method, room_number) values"
                "(?, ?, ?, ?, ?, ?)");
            pstmt->setInt(1, *ID);
            pstmt->setString(2, *name);
            pstmt->setInt(3, *phone_number);
            pstmt->setDateTime(4, *exit_date);
            if (*Pmethod == 'B')
                payment_method = "Bank Of Abyssinia";
            else if (*Pmethod == 'C')
                payment_method = "Commercial Bank of Ethiopia";
            else if (*Pmethod == 'T')
                payment_method = "Tell-Birr";
            else
                payment_method = "M-Pesa";
            pstmt->setString(5, payment_method);
            pstmt->setInt(6, *room_number);
            pstmt->execute();
            delete pstmt;
        }
        delete con;
    }
    catch (SQLException& e) {
        cout << "Error: " << e.what() << endl;
        return;
    }
}

void roomPreferance(int* room_floor, float* price) {
    try {
        string status;
        Connection* con = connectionEstablisher();
        PreparedStatement* pstmt = con->prepareStatement("select * from rooms where room_floor = ? and room_price < ?");
        pstmt->setInt(1, *room_floor);
        pstmt->setDouble(2, *price);
        ResultSet* res = pstmt->executeQuery();
     
        cout << "---------------------------------------------------------------------------------------------------\n";
        cout << "Room Number" << setw(20) << "Type" << setw(24) << "Price per day" << setw(24) << "Floor" << setw(16) << "Status" << endl;
        cout << "-----------" << setw(20) << "----" << setw(24) << "-------------" << setw(24) << "-----" << setw(16) << "------" << endl;
        while (res->next()) {
            if (roomStatusChecker(res->getInt("room_number")))
                status = "Not Available";
            else
                status = "Available";
            cout << setw(7) << res->getString(1) << setw(24) << res->getString(2) << setw(17) << res->getDouble(3) << " Birr" << setw(24) << res->getString(4) << setw(20) << status << endl;
        }
        cout << "---------------------------------------------------------------------------------------------------\n";
        delete res;
        delete pstmt;
        delete con;
    }
    catch (SQLException& e) {
        cout << "Error: " << e.what() << endl;
        return;
    }
}

void showAllAvailableRooms() {
    try {
        Connection* con = connectionEstablisher();
        Statement* stmt = con->createStatement();

        string getAllAvailable = "select rooms.* from rooms where rooms.room_number not in (select customer.room_number from customer)";
        ResultSet* res = stmt->executeQuery(getAllAvailable);
        
        cout << "---------------------------------------------------------------------------------------------\n";
        cout << "Room Number" << setw(20) << "Type" << setw(24) << "Price per day" << setw(24) << "Floor" << endl;
        cout << "-----------" << setw(20) << "----" << setw(24) << "-------------" << setw(24) << "-----" << endl;
        while (res->next()) {
            cout << setw(7) << res->getString(1) << setw(24) << res->getString(2) << setw(17) << res->getDouble(3) << " Birr" << setw(24) << res->getString(4) << endl;
        }
        cout << "---------------------------------------------------------------------------------------------\n";
        delete res;
        delete stmt;
        delete con;
    }
    catch (SQLException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void deleteBooked(int* room_number) {
    try {
        Connection* con = connectionEstablisher();
        PreparedStatement* pstmt = con->prepareStatement("delete from customer where room_number = ?");
        pstmt->setInt(1, *room_number);
        pstmt->execute();
    
        delete pstmt;
        delete con;
    }
    catch (SQLException& e) {
        cout << "Error: " << e.what() << endl;
        return;
    }
}

void changeRoom(int* currRN, int* newRM) {
    try {
        Connection* con = connectionEstablisher();
        PreparedStatement* getData = con->prepareStatement("select * from customer where room_number = ?");
        getData->setInt(1, *currRN);
        ResultSet* customer = getData->executeQuery();
    
        deleteBooked(currRN);
        if (customer->next()) {
            int ID = customer->getInt("customer_id");
            string name = customer->getString("full_name");
            int phone_number = customer->getInt("phone_number");
            string exit_date = customer->getString("exit_date");
            string Pmethod = customer->getString("payment_method"); char PM = Pmethod[0];
            int room_number = customer->getInt("room_number");
            reserveRoom(&ID, &name, &phone_number, &exit_date, &PM, newRM);
        }
        delete con;
        delete customer;
        delete getData;
    }
    catch (SQLException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

//admin

void searchBookedRoom(int* room_number) {
    try {
        bool isBooked = false;
        Connection* con = connectionEstablisher();
        PreparedStatement* pstmtC = con->prepareStatement("select * from customer where room_number = ?");
        PreparedStatement* pstmtR = con->prepareStatement("select * from rooms where room_number = ?");
        pstmtC->setInt(1, *room_number);
        pstmtR->setInt(1, *room_number);
        ResultSet* customers = pstmtC->executeQuery();
        ResultSet* rooms = pstmtR->executeQuery();
      
        if (customers->next() && rooms->next()) {
            cout << "\t\t--------------------------------------------------------------------------------------\n";
            cout << "\t\t\tRoom details\t\t\tBooked by\n";
            cout << "\t\t\t------------\t\t\t\t---------\n";
            cout << "\t\t\tRoom number: \t" << rooms->getString("room_number") << "\t\t" << "ID: \t\t\t" << customers->getString("customer_id") << endl;
                
            cout << "\t\t\tRoom type: \t" << rooms->getString("room_type") << "\t\t" << "Name: \t\t\t" << customers->getString("full_name") << endl;
            cout << "\t\t\tRoom price: \t" << rooms->getDouble("room_price") << " birr\t" << "Phone number: \t\t+251-" << customers->getString("phone_number") << endl;
            cout << "\t\t\tRoom floor: \t" << rooms->getString("room_floor") << "\t\t" << "Entry date: \t\t" << customers->getString("entry_date") << endl;
            cout << "\t\t\t\t\t\t\t" << "Exit date: \t\t" << customers->getString("exit_date") << endl;
            cout << "\t\t\t\t\t\t\t" << "Payment method: \t" << customers->getString("payment_method") << endl;
            cout << "\t\t--------------------------------------------------------------------------------------\n";
        }
        else {
            cout << "\n\t\t+-----------------------------------------------+\n"
                    "\t\t|        Room number "+ to_string(*room_number) +" is NOT booked!!        |\n"
                    "\t\t+-----------------------------------------------+\n";
        }
        delete customers;
        delete rooms;
        delete pstmtR;
        delete pstmtC;
        delete con;
    }
    catch (SQLException& e) {
        cout << "Error: " << e.what() << endl;
        return;
    }
}

void showAllBookedRooms() {
    try {
        Connection* con = connectionEstablisher();
        Statement* stmt = con->createStatement();

        string getBookedRooms = "select rooms.*  from rooms where rooms.room_number in (select customer.room_number from customer) order by room_number";
        string getCustomers = "select * from customer order by room_number";
        ResultSet* rooms = stmt->executeQuery(getBookedRooms);
        ResultSet* customers = stmt->executeQuery(getCustomers);
     
        while (rooms->next() && customers->next()) {
            cout << "\t\t-----------------------------------------------------------------------------------------\n";
            cout << "\t\t\tRoom details\t\t\tBooked by\n";
            cout << "\t\t\t------------\t\t\t---------\n";
            cout << "\t\t\tRoom number: \t" << rooms->getString("room_number") << "\t\t" << "ID: \t\t\t" << customers->getString("customer_id") << endl;
            cout << "\t\t\tRoom type: \t" << rooms->getString("room_type") << "\t\t" << "Name: \t\t\t" << customers->getString("full_name") << endl;
            cout << "\t\t\tRoom price: \t" << rooms->getString("room_price") << " birr\t" << "Phone number: \t\t+251-" << customers->getString("phone_number") << endl;
            cout << "\t\t\tRoom floor: \t" << rooms->getString("room_floor") << "\t\t" << "Entry date: \t\t" << customers->getString("entry_date") << endl;
            cout << "\t\t\t\t\t\t\t" << "Exit date: \t\t" << customers->getString("exit_date") << endl;
            cout << "\t\t\t\t\t\t\t" << "Payment method: \t" << customers->getString("payment_method") << endl;
            cout << "\t\t-----------------------------------------------------------------------------------------\n";
        }
        system("pause>0");

        delete con;
        delete rooms;
        delete customers;
        delete stmt;
    }
    catch (SQLException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void updateRoom(int* Old_RN, int* N_ID, string* N_name, int* N_PN, string* N_exitDate, char* N_Pmethod) {
    try {
    Connection* con = connectionEstablisher();
    PreparedStatement* pstmt = con->prepareStatement("delete from customer where room_number = ?");
    pstmt->setInt(1, *Old_RN);
    pstmt->execute();
   
    reserveRoom(N_ID, N_name, N_PN, N_exitDate, N_Pmethod, Old_RN);
    delete con;
    delete pstmt;
    }
    catch (SQLException& e) {
        cout << "Error: " << e.what() << endl;
    }

}

void showAllRoomsA() {
    try {
        Connection* con = connectionEstablisher();
        Statement* stmt = con->createStatement();
        string status;
        string getAllAvailable = "select * from rooms";
        ResultSet* res = stmt->executeQuery(getAllAvailable);
      
        cout << "---------------------------------------------------------------------------------------------------\n";
        cout << "Room Number" << setw(20) << "Type" << setw(24) << "Price per day" << setw(24) << "Floor" << setw(16) << "Status" << endl;
        cout << "-----------" << setw(20) << "----" << setw(24) << "-------------" << setw(24) << "-----" << setw(16) << "------" << endl;
        while (res->next()) {
            if (roomStatusChecker(res->getInt("room_number")))
                status = "BOOKED";
            else
                status = "NOT BOOKED";
            cout << setw(7) << res->getString(1) << setw(24) << res->getString(2) << setw(17) << res->getDouble(3) << " Birr" << setw(24) << res->getString(4) << setw(20) << status << endl;
        }
        cout << "---------------------------------------------------------------------------------------------------\n";

        delete res;
        delete stmt;
        delete con;
    }
    catch (SQLException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void AddAdmin(const Admin& admin) {
    try{
        Connection* con = connectionEstablisher();
        PreparedStatement* pstmt = con->prepareStatement("insert into adminstrator(admin_full_name, admin_email, admin_username, admin_password)"
            "values(?, ?, ?, ?)");
        pstmt->setString(1, admin.full_name);
        pstmt->setString(2, admin.email);
        pstmt->setString(3, admin.username);
        pstmt->setString(4, admin.password);
        pstmt->execute();
      
        cout << "\n\t\t\t\t\t     +----------------------------+\n"
            "\t\t\t\t\t     |         ADMIN ADDED        |\n"
            "\t\t\t\t\t     +----------------------------+\n";
    }
    catch (SQLException& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void deleteAdmin(string* username) {
    try {
        bool adminFound = false;
        Connection* con = connectionEstablisher();
        PreparedStatement* pstmt = con->prepareStatement("delete from adminstrator where admin_username = ?");
        Statement* stmt = con->createStatement();
        string getAdmins = "select admin_username from adminstrator";
        ResultSet* res = stmt->executeQuery(getAdmins);
      
        while (res->next()) {

            if (res->getString("admin_username") == *username) {
                pstmt->setString(1, *username);
                pstmt->execute();
                cout << "\n\t\t\t\t\t     +---------------------------+\n"
                    "\t\t\t\t\t     |         DELETED!!!        |\n"
                    "\t\t\t\t\t     +---------------------------+\n";
                adminFound = true;
                break;
            }
        }
        if (!adminFound) {
            cout << "\n\t\t\t\t\t     +--------------------------------+\n"
                "\t\t\t\t\t     |         ADMIN NOT FOUND        |\n"
                "\t\t\t\t\t     +--------------------------------+\n";
        }
    }
    catch (SQLException& e) {
        cout << "Error: " << e.what() << endl;
    }
}
