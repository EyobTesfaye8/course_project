#include "sql.h"

void add_admin() {
    string conf_pass;char choice;
    Admin admin;
    cout << "+----------------------------------------------------------------------------------------------------------------------+\n";
    cout << "\t\t\t\t\t+---------------------------------------+\n"
        "\t\t\t\t\t|          ADDING ADMIN ACCOUNT         |\n"
        "\t\t\t\t\t+---------------------------------------+\n";
    cout << "\tProvide the information of the new adminstrator below:\n\n";
    cout << "\t\tEnter full name:  ";
    cin >> admin.full_name;
    cin.ignore(1000, '\n');
    cout << "\t\tEnter your email:  ";
    cin >> admin.email;
    cin.ignore(1000, '\n');
    cout << "\t\tEnter username:  ";
    cin >> admin.username;
    cin.ignore(1000, '\n');
    lable:
    cout << "\t\tEnter new password:  ";
    cin >> admin.password;
    cout << "\t\tConfirm your password:  ";
    cin >> conf_pass;
    if (conf_pass == admin.password) {
        AddAdmin(admin);
        cout << "+----------------------------------------------------------------------------------------------------------------------+\n";
    }
    else {
        cout << "\t\tThe password don't match!!";
        cout << "\n\t\tDo you want to try again? (y/any character):  ";
        cin >> choice;
        if (!(toupper(choice) == 'Y')) {
            cout << "\n\t\t\t\t\t     +---------------------------+\n"
                "\t\t\t\t\t     |         CANCELED!!        |\n"
                "\t\t\t\t\t     +---------------------------+\n";
            cout << "+----------------------------------------------------------------------------------------------------------------------+\n";
            return;
        }
        goto lable;
    }

}

void delete_admin(string* CURR_admin_username) {
    string username, password;
    cout << "+----------------------------------------------------------------------------------------------------------------------+\n";
    cout << "\t\t\t\t\t  +-----------------------------------------+\n"
        "\t\t\t\t\t  |          DELETING ADMIN ACCOUNT         |\n"
        "\t\t\t\t\t  +-----------------------------------------+\n";
    lable:
    cout << "\n\t\tEnter the username of the admin:  ";
    cin >> username;
    cout << "\n\t\tEnter your password:  ";
    cin >> password;
    if (password == *CURR_admin_username) {
        deleteAdmin(&username);
    }
    else {
        char choice;
        cout << "\t\tIncorrect password!!";
        cout << "\n\t\tDo you want to try again? (y/any character):  ";
        cin >> choice;
        if (!(toupper(choice) == 'Y')) {
            cout << "\n\t\t\t\t\t     +---------------------------+\n"
                "\t\t\t\t\t     |         CANCELED!!        |\n"
                "\t\t\t\t\t     +---------------------------+\n";
            cout << "+----------------------------------------------------------------------------------------------------------------------+\n";
            return;
        }
        goto lable;
    }

}

bool dateVerifier(string d) {
    return isdigit(d[0]) && isdigit(d[1]) && isdigit(d[2]) && isdigit(d[3]) && 
        d[4] == '/' && isdigit(d[5]) && isdigit(d[6]) && d[7] == '/' && 
        isdigit(d[8]) && isdigit(d[9]);
}

void search_room() {
    int room_number;
    cout << "\n\t\tEnter the room number:  ";
    cin >> room_number;
    while (cin.fail()) {
        cout << "\t\tEnter a valid room number:  ";
        cin.clear();
        cin.ignore(1000, '\n');
        cin >> room_number;
    }
    if (roomStatusChecker(room_number))
        searchBookedRoom(&room_number);
    else {
        cout << "\n\t\t\t     +-----------------------------------------------------------------+\n"
            "\t\t\t     |        Room number " + to_string(room_number) + " is NOT booked or does NOT exist!!        |\n"
            "\t\t\t     +-----------------------------------------------------------------+\n";
    }
}

void get_data() {
    int customer_id, PhoneNumber,room_number;
    string name, exit_date;
    char payment_method, ch;
    do {
        cout << "+----------------------------------------------------------------------------------------------------------------------+\n";
        cout << "\t\t\t\t\t     +---------------------------+\n"
            "\t\t\t\t\t     |          BOOKING          |\n"
            "\t\t\t\t\t     +---------------------------+\n";
        cout << "\tPlease fill out the form below.\n";
        cout << "\n\t\tEnter your ID:  ";
        cin >> customer_id;
        while (cin.fail()) {
            cout << "\t\tEnter a valid ID:  ";
            cin.clear();
            cin.ignore(1000, '\n');
            cin >> customer_id;
        }
        cout << "\t\tEnter your full name:  ";
        cin >> name;
        cin.ignore(1000, '\n');
        cout << "\t\tEnter room number:  ";
        cin >> room_number;
        while (cin.fail()) {
            cout << "\t\tEnter a valid room number:  ";
            cin.clear();
            cin.ignore(1000, '\n');
            cin >> room_number;
        }
        cout << "\t\tEnter your phone number:  +251-";
        cin >> PhoneNumber;
        while (cin.fail() || (PhoneNumber < 700000000 && PhoneNumber > 799999999) || (PhoneNumber < 900000000 && PhoneNumber > 999999999)) {
            cout << "\t\tInvalid phone number!\n\t\tEnter a valid phone number:  +251-";
            cin.clear();
            cin.ignore(1000, '\n');
            cin >> PhoneNumber;
        }
        cout << "\t\tEnter check-out date (YYYY/MM/DD):  ";
        cin >> exit_date;
        while (!dateVerifier(exit_date)) {
            cout << "\t\tInvalid date!!\n\t\tEnter a valid date (YYYY/MM/DD):  ";
            cin.clear();
            cin.ignore(1000, '\n');
            cin >> exit_date;
        }

        cout << "\t\tPayment options\n";
        cout << "\t\t   T - Tele Birr\n";
        cout << "\t\t   M - M-Pesa\n";
        cout << "\t\t   C - CBE   \n";
        cout << "\t\t   B - Bank Of Abyssinia  \n";
        cout << "\t\tSelect your payment method:   ";
        string correctChooices = "tbcmTBCM";
        cin >> payment_method;
        while (cin.fail() || (correctChooices.find(payment_method) == string::npos)) { // string::npos = "not found"
            cout << "\t\t\tInvalid choice!!\n\t\t\tPlease choose again:  ";
            cin.clear();
            cin.ignore(1000, '\n');
            cin >> payment_method;
        }
        payment_method = toupper(payment_method);
        if (!roomStatusChecker(room_number)) {
            reserveRoom(&customer_id, &name, &PhoneNumber, &exit_date, &payment_method, &room_number); // call the function reserveRoom from mysql.cpp
            cout << "\n\t\t+-------------------------------------------------------+\n"
                "\t\t|        You have booked room " + to_string(room_number) + " successfully!!        |\n"
                "\t\t+-------------------------------------------------------+\n";
        }
        else {
            cout << "\n\t\t+--------------------------------------------+\n"
                "\t\t|        Room " + to_string(room_number) + " is already booked!!        |\n"
                "\t\t+--------------------------------------------+\n";
        }

        cout << "\n\t\tDo you want to book another room? (y/any other key)  ";
        cin >> ch;
        while (cin.fail()) {
            cout << "\t\tInvalid choice!!\n\t\tPlease enter a valid choice:  ";
            cin.clear();
            cin.ignore(1000, '\n');
            cin >> ch;
        }
        ch = toupper(ch);
        cout << "+----------------------------------------------------------------------------------------------------------------------+\n";
    } while (ch == 'Y');
}

void preferRoom() {
    int room_floor;
    float room_price;
    cout << "which floor do you prefer(1-5):  ";
    cin >> room_floor;
    while (cin.fail() || room_floor < 1 || room_floor>5) {
        cout << "Please enter a valid floor number (1-5): ";
        cin.clear();
        cin.ignore(1000, '\n');
        cin >> room_floor;
    }
    cout << "Price is less than (min 1600):  ";
    cin >> room_price;
    while (cin.fail() || room_price < 1600) {
        if (cin.fail())
            cout << "Enter a Valid Price:  ";
        else
            cout << "The minimum price is 1600. Enter again:  ";
        cin.clear();
        cin.ignore(1000, '\n');
        cin >> room_price;
    }
    roomPreferance(&room_floor, &room_price);
}

void update_data() {
    int O_roomNumber;
    int customer_id, PhoneNumber, room_number;
    string name, exit_date;
    char payment_method, ch;
    do {
        cout << "+----------------------------------------------------------------------------------------------------------------------+\n";
        cout << "\t\t\t\t\t     +---------------------------+\n"
            "\t\t\t\t\t     |          UPDATING         |\n"
            "\t\t\t\t\t     +---------------------------+\n";
        cout << "\tPlease fill out the form below.\n";
        cout << "\n\t\tEnter the room number you want to update: ";
        cin >> O_roomNumber;
        while (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\t\tEnter a valid room number:  ";
            cin >> O_roomNumber;
        }
        if (roomStatusChecker(O_roomNumber)) {
            cout << "\n\t\tThe room you are updating is \n\n";
            searchBookedRoom(&O_roomNumber);

            cout << "\n\n\t\tEnter your New ID:  ";
            cin >> customer_id;
            while (cin.fail()) {
                cout << "\t\tEnter a valid ID:  ";
                cin.clear();
                cin.ignore(1000, '\n');
                cin >> customer_id;
            }
            cout << "\t\tEnter your New name:  ";
            cin >> name;
            cin.ignore(1000, '\n');
            cout << "\t\tEnter your phone number:  +251-";
            cin >> PhoneNumber;
            while (cin.fail() || (PhoneNumber < 700000000 && PhoneNumber > 799999999) || (PhoneNumber < 900000000 && PhoneNumber > 999999999)) {
                cout << "\t\tInvalid phone number!\n\t\tEnter a valid phone number:  +251-";
                cin.clear();
                cin.ignore(1000, '\n');
                cin >> PhoneNumber;
            }
            cout << "\t\tEnter check-out date (YYYY/MM/DD):  ";
            cin >> exit_date;
            while (!dateVerifier(exit_date)) {
                cout << "\t\tInvalid date!!\n\t\tEnter a valid date (YYYY/MM/DD):  ";
                cin.clear();
                cin.ignore(1000, '\n');
                cin >> exit_date;
            }

            cout << "\t\tPayment options\n";
            cout << "\t\t   T - Tele Birr\n";
            cout << "\t\t   M - M-Pesa\n";
            cout << "\t\t   C - CBE   \n";
            cout << "\t\t   B - Bank Of Abyssinia  \n";
            cout << "\t\tSelect your payment method:   ";
            string correctChooices = "tbcmTBCM";
            cin >> payment_method;
            while (cin.fail() || (correctChooices.find(payment_method) == string::npos)) {
                cout << "\t\t\tInvalid choice!!\n\t\t\tPlease choose again:  ";
                cin.clear();
                cin.ignore(1000, '\n');
                cin >> payment_method;
            }
            payment_method = toupper(payment_method);
            updateRoom(&O_roomNumber, &customer_id, &name, &PhoneNumber, &exit_date, &payment_method);
            cout << "\n\t\t\t      +---------------------------------------------------------+\n"
                "\t\t\t      |        Room number " + to_string(O_roomNumber) + " is updated successfully!!        |\n"
                "\t\t\t      +---------------------------------------------------------+\n";
        }
        else {
            cout << "\n\t\t\t     +-----------------------------------------------------------------+\n"
                "\t\t\t     |        Room number " + to_string(O_roomNumber) + " is NOT booked or does NOT exist!!        |\n"
                "\t\t\t     +-----------------------------------------------------------------+\n";
        }
        cout << "\n\t\tDo you want to update another room? (y/any other key)  ";
        cin >> ch;
        while (cin.fail()) {
            cout << "\t\tInvalid choice!!\n\t\tPlease enter a valid choice:  ";
            cin >> ch;
        }
        ch = toupper(ch);
        cout << "+----------------------------------------------------------------------------------------------------------------------+\n";
    } while (ch == 'Y');
}

void unbookRoom() {
    bool confirmation = false;
    int roomNumber;
    char conf;
    do {
        cout << "+----------------------------------------------------------------------------------------------------------------------+\n";
        cout << "\t\t\t\t\t     +---------------------------+\n"
            "\t\t\t\t\t     |         UNBOOKING         |\n"
            "\t\t\t\t\t     +---------------------------+\n";
        cout << "\t\tThe room number you want to unbook:  ";
        cin >> roomNumber;
        while (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\t\tEnter a valid room number:  ";
            cin >> roomNumber;
        }
        if (roomStatusChecker(roomNumber)) {
            searchBookedRoom(&roomNumber);
            cout << "\n\t\tAre you sure you want to unbook this room(y/any character):  ";
            cin >> conf;
            while (cin.fail()) {
                cin.clear();
                cin.ignore();
                cout << "\t\t\tEnter a valid choice:  ";
                cin >> conf;
            }
            if (toupper(conf) == 'Y') {
                confirmation = true;
                deleteBooked(&roomNumber);
                cout << "\n\t\t+----------------------------------------------------------+\n"
                    "\t\t|         You have unbooked room " + to_string(roomNumber) + " successfully!!        |\n"
                    "\t\t+----------------------------------------------------------+\n";
            }
            else {
                cout << "\n\t\t\t\t\t     +---------------------------+\n"
                    "\t\t\t\t\t     |         CANCELED!!        |\n"
                    "\t\t\t\t\t     +---------------------------+\n";
            }
        }
        else {
            cout << "\n\t\t+-----------------------------------------------+\n"
                "\t\t|        Room number " + to_string(roomNumber) + " is NOT booked!!        |\n"
                "\t\t+-----------------------------------------------+\n";
        }
        cout << "\t\t\tDo you want to unbook another room? (y/any character)  ";
        cin >> conf;
        while (cin.fail()) {
            cin.clear();
            cin.ignore();
            cout << "\t\t\tEnter a valid choice:  ";
            cin >> conf;
        }
        cout << "+----------------------------------------------------------------------------------------------------------------------+\n";
    } while (toupper(conf) == 'Y');
}

void oldRoomTerminator() {
    Connection* con = connectionEstablisher();
    int currentRoomNo, newRoomNo, ID;
    char conf;
    cout << "+----------------------------------------------------------------------------------------------------------------------+\n";
    cout << "\t\t\t\t\t     +--------------------------------+\n"
        "\t\t\t\t\t     |          CHANGING ROOM         |\n"
        "\t\t\t\t\t     +--------------------------------+\n";
    cout << "\t\t\tEnter your ID:   ";
    cin >> ID;
    while (cin.fail()) {
        cout << "\t\t\tEnter a Valid ID:  ";
        cin.clear();
        cin.ignore(1000, '\n');
        cin >> ID;
    }
    cout << "\t\t\tEnter your current room number:   ";
    cin >> currentRoomNo;
    while (cin.fail()) {
        cout << "\t\t\tEnter a Valid Room Number:   ";
        cin.clear();
        cin.ignore(1000, '\n');
        cin >> currentRoomNo;
    }
    if (roomStatusChecker(currentRoomNo)) {
        PreparedStatement* pstmt = con->prepareStatement("select customer_id from customer where room_number = ?");
        pstmt->setInt(1, currentRoomNo);
        ResultSet* c_id = pstmt->executeQuery();
        RecoredLog();
        if (c_id->next()) {
            if (ID == c_id->getInt("customer_id")) {
                char pref_conf;
                cout << "\n\t\tDo you want to see available rooms?(y/any character):  ";
                cin >> pref_conf;
                if (toupper(pref_conf) == 'Y') {
                    showAllAvailableRooms();
                    cout << "\n\t\tDo you want to add filter? (y/any character):   ";
                    cin >> pref_conf;
                    while (cin.fail()) {
                        cout << "\t\tEnter a valid answer: ";
                        cin.clear();
                        cin.ignore();
                        cin >> pref_conf;
                    }
                    if (toupper(pref_conf) == 'Y') {
                        do {
                            preferRoom();
                            cout << "\t\tEnter 'q' to exit or any other character to continue:  ";
                            cin >> pref_conf;
                        } while (toupper(pref_conf) != 'Q');
                    }
                }
                cout << "\t\t\tEnter the room number you want to move into:  ";
                cin >> newRoomNo;
                while (cin.fail()) {
                    cout << "\t\t\tEnter a Valid Room Number: ";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cin >> newRoomNo;
                }
                aboutRoom(&newRoomNo);
                cout << "\n\t\tDo you want to move to this room? (y/any character):   ";
                cin >> conf;
                if (toupper(conf) == 'Y') {
                    changeRoom(&currentRoomNo, &newRoomNo);
                    cout << "\n\t\t\t\t+-----------------------------------------------------+\n"
                        "\t\t\t\t|        You have changed you room to " + to_string(newRoomNo) + "!!           |\n"
                        "\t\t\t\t+-----------------------------------------------------+\n";
                    cout << "+----------------------------------------------------------------------------------------------------------------------+\n";
                    return;
                }
                else {
                    cout << "\n\t\t\t\t\t     +---------------------------+\n"
                        "\t\t\t\t\t     |         CANCELED!!        |\n"
                        "\t\t\t\t\t     +---------------------------+\n";
                }
            }
            else {
                cout << "\n\t\t\t\t     +-----------------------------------------------+\n"
                    "\t\t\t\t     |        You did not book room " + to_string(currentRoomNo) + "!!            |\n"
                    "\t\t\t\t     +-----------------------------------------------+\n";
            }
        }
    }
    else {
        cout << "\n\t\t\t\t     +-----------------------------------------------+\n"
            "\t\t\t\t     |        You did not book room " + to_string(currentRoomNo) + "!!            |\n"
            "\t\t\t\t     +-----------------------------------------------+\n";
        cout << "+----------------------------------------------------------------------------------------------------------------------+\n";
    }
}
