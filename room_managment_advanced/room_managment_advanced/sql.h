#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <iostream>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <iomanip>

using namespace std;
using namespace sql;
using namespace mysql;

struct Admin {
	string full_name, email, username, password;
};
void RecoredLog();
//database handler
Connection* connectionEstablisher();
void reserveRoom(int*, string*, int*, string*, char*, int*);
void deleteBooked(int*);
void searchBookedRoom(int*);
bool checkAdmin(string*, string*);
void roomPreferance(int*, float*);
void showAllAvailableRooms();
void showAllBookedRooms();
bool roomStatusChecker(int);
void updateRoom(int*, int*, string*, int*, string*, char*);
void changeRoom(int*, int*);
void aboutRoom(int*);
void showAllRoomsA();
void AddAdmin(const Admin&);
void deleteAdmin(string*);
//user input handler
void get_data();
void update_data();
bool dateVerifier(string);
void unbookRoom();
void search_room();
void Adminstrator(string*, string*);
void Customer();
void preferRoom();
void oldRoomTerminator();
void add_admin();
void delete_admin(string*);
