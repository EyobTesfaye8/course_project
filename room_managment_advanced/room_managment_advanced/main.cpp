#include "sql.h"

int main() {
    int choice;
	lable:
    cout << "+----------------------------------------------------------------------------------------------------------------------+\n"
            "|                //``\\  +---  |``\\\\  +---  +---  +---                |\\  /|  /``\\\\  -------  +---   |                  |\n"
            "|                |  __  |--   |   |  |--   |--   |--                 | \\/ |  |   |     |     |--    |                  |\n"
            "|                \\\\__|  +---  |__//  +---  |     |                   |    |  \\\\__/     |     +---   +---               |\n"
            "+----------------------------------------------------------------------------------------------------------------------+\n";
    cout << "\t\t\t\t\t    |      ~~~~~~~~~~~~~~~~~~~~     |\n"
            "\t\t\t\t\t    |      WELCOME TO OUR MOTEL     |\n"
            "\t\t\t\t\t    +-------------------------------+\n";
		cout << "\t\t\t\tDEAR USER, HOW DO YOU WANT TO ACCESS THE SYSTEM: \n";
		cout << "\t\t\t\t\t1 - As an admin\n";
		cout << "\t\t\t\t\t2 - As a Customer\n";
		cout << "\t\t\t\t\t3 - exit\n";
		cout << "\t\t\t\tEnter your choice:  ";
		cin >> choice;
		while (cin.fail() || choice > 3 || choice < 1) {
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "\t\t\t\tInvalid choice. choose again:  ";
			cin >> choice;
		}
		if (choice == 1) {
			string username, password;
			int attempts = 3;
			cout << "\n\t\t\t\t      +--------------------------------------------+\n"
				"\t\t\t\t      |            Loging in as an Admin           |\n"
				"\t\t\t\t      +--------------------------------------------+\n";
			while (attempts--) {
				cout << "\t\t\t\t\tEnter username: ";
				cin >> username;
				cout << "\t\t\t\t\tEnter password: ";
				cin >> password;

				if (checkAdmin(&username, &password)) {
					system("cls");
					Adminstrator(&username, &password);
					attempts = 3;
					goto lable;
				}
				else {
					cout << "\t\t\t\tIncorrect username or password. ";
					if (attempts > 0) {
						cout << "\n\t\t\t\t" << attempts << " attempts left.\n";
					}
				}
			}
			cout << "\n\t\t\tToo many failed attempts. Access denied.\n";
			exit(0);
		}
		else if (choice == 2) {
			system("cls");
			Customer();
		}
}

void Adminstrator(string* username, string* password) {
	int choice;
	string adminFullName;
	Connection* con = connectionEstablisher();
	PreparedStatement* pstmt = con->prepareStatement("select admin_full_name from adminstrator where admin_username = ?");
	pstmt->setString(1, *username);
	ResultSet* res = pstmt->executeQuery();
	RecoredLog();
	if (res->next()) {
		adminFullName = res->getString(1);
	}
	cout <<"== ADMIN ===========================----------------------------------------------------================================" << endl;
    cout << "\t\t\t\t    +--------------------------------------------------+\n"
            "\t\t\t\t                  WELCOME, "+adminFullName+"\n"
            "\t\t\t\t    +--------------------------------------------------+\n";
	while (true) {
		cout << "\t\tYou may choose from the menu.\n";
		cout << "\t\t\t1 - Book a room\t\t\t\t\t2 - Unbook a room\n"
			    "\t\t\t3 - Update a room\t\t\t\t4 - Show all booked rooms and their status\n"
			    "\t\t\t5 - Search a room\t\t\t\t6 - Show all rooms and their status\n"
			    "\t\t\t7 - Manage admins\t\t\t\t8 - Sign out\n";
		cout << "\n\t\t\tYour choice:  "; cin >> choice;
		while (cin.fail() || choice > 7 || choice < 1) {
			cin.clear();
			cin.ignore(1000, '\n');
			cin >> choice;
		}
		switch (choice)
		{
		case 1: get_data(); break;
		case 2: unbookRoom(); break;
		case 3: update_data(); break;
		case 4: showAllBookedRooms(); break;
		case 5: search_room(); break;
		case 6: showAllRoomsA(); break;
		case 7:cout << "\n\t\t\t\t\t     +--------------------------------+\n"
			"\t\t\t\t\t     |         MANAGING ADMINS        |\n"
			"\t\t\t\t\t     +--------------------------------+\n\n";
			int choice;
			cout << "\t\t\t1 - Add admin\n";
			cout << "\t\t\t2 - Delete admin\n";
			cout << "\t\t\tYour choice:  ";
			cin >> choice;
			while (cin.fail() || choice > 2 || choice < 1) {
				cin.clear();
				cin.ignore(1000, '\n');
				cin >> choice;
			}
			if (choice == 1)
				add_admin();
			else
				delete_admin(password);
			break;
		case 8: cout << "\t\t\t\t\t     +-----------------------------+\n"
			"\t\t\t\t\t     |          SIGNED OUT         |\n"
			"\t\t\t\t\t     +-----------------------------+\n";
			return;
		}
	}
}

void  Customer() {
	cout << "== CUSTOMER ========================----------------------------------------------------================================" << endl;
	cout << "\t\t\t\t    +--------------------------------------------------+\n"
		"\t\t\t\t    |            WELCOME, DEAR CUSTOMER!!              |\n"     
		"\t\t\t\t    +--------------------------------------------------+\n";
	while (true) {
		cout << "\t\tYou may choose from the menu below: ";
		cout << "\n\n\t\t\t1 - Book a Room"
			"\n\t\t\t2 - Show All Available Rooms"
			"\n\t\t\t3 - Change room"
			"\n\t\t\t4 - Sign out";
		int choice;
		cout << "\n\t\t\tEnter Your Choice: ";
		cin >> choice;
		while (cin.fail() || choice > 4 || choice < 1) {
			cout << "\t\t\tEnter a Valid Choice: ";
			cin.clear();
			cin.ignore(1000, '\n');
			cin >> choice;
		}
		switch (choice) {
		case 1: get_data(); break; 
		case 2: showAllAvailableRooms();
			char pref_conf;
			cout << "\n\t\tDo you want to add filter? (y/any character):   ";
			cin >> pref_conf;
			while (cin.fail()) {
				cout << "\t\tEnter a valid answer: ";
				cin.clear();
				cin.ignore();
				cin >> pref_conf;
			}
			if (toupper(pref_conf) == 'Y') {
				do{
					preferRoom();
					cout << "\t\tEnter 'q' to exit or any other character to continue:  ";
					cin >> pref_conf;
				} while (toupper(pref_conf) != 'Q');
			}
				break;
		case 3: oldRoomTerminator(); break;
		case 4: cout << "\t\t\t\t  +-------------------------------------------------+\n"
			"\t\t\t\t  |         THANK YOU FOR USING OUR SERVICE         |\n"
			"\t\t\t\t  +-------------------------------------------------+\n";
			return;
		}
	}
}
