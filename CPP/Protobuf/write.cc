#include <iostream>
#include <fstream>
#include <string>
#include "addressbook.pb.h"
using namespace std;
// This function fills in a Person message based on user input.

/*
getline()的原型是istream& getline ( istream &is , string &str , char delim );

其中 istream &is 表示一个输入流，譬如cin；

string&str表示把从输入流读入的字符串存放在这个字符串中（可以自己随便命名，str什么的都可以）；

char delim表示遇到这个字符停止读入，在不设置的情况下系统默认该字符为'\n'，也就是回车换行符（遇到回车停止读入）。

ps: 对于while(getline(cin,line)) 语句，这里默认回车符停止读入,按Ctrl+Z或键入EOF回车即可退出循环。

*/
void PromptForAddress(tutorial::Person *person) {
    cout << "Enter person ID number: ";
    int id;
    cin >> id;
    person->set_id(id);
    cin.ignore(256, '\n');//忽略最后的回车

    cout << "Enter name: ";
    string name;
    //getline(cin, *person->mutable_name());//自动到"\n"停止读入,自动忽略
    cin>>name;
    //person->set_name(name);
    *person->mutable_name()=name;//mutable_name 表示name这个成员
    cout << "Enter email address (blank for none): ";
    string email;
   // getline(cin, email);
    cin >> email;
    if (!email.empty()) {
        person->set_email(email);
    }
     cin.ignore(256, '\n');//cin输入会包含\n相当于enter,会导致在下文直接blank leave,因此需要忽略最后的回车
    while (true) {
        cout << "Enter a phone number (or leave blank to finish): ";
        string number;
        getline(cin, number);
        if (number.empty()) {
            break;
        }
        tutorial::Person::PhoneNumber *phone_number = person->add_phones();
        phone_number->set_number(number);
        cout << "Is this a mobile, home, or work phone? ";
        string type;
        getline(cin, type);
        if (type == "mobile") {
            phone_number->set_type(tutorial::Person::MOBILE);
        } else if (type == "home") {
            phone_number->set_type(tutorial::Person::HOME);
        } else if (type == "work") {
            phone_number->set_type(tutorial::Person::WORK);
        } else {
            cout << "Unknown phone type.  Using default." << endl;
        }
    }
}
// Main function:  Reads the entire address book from a file,
//   adds one person based on user input, then writes it back out to the same
//   file.
int main() {
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;
//    if (argc != 2) {
//        cerr << "Usage:  " << argv[0] << " ADDRESS_BOOK_FILE" << endl;
//        return -1;
//    }
  // string argv[1] = "xiaohaipeng.prototxt";
    string argv= "./phonebook.prototxt";
    tutorial::AddressBook address_book;
    {
        // Read the existing address book.
        fstream input(argv, ios::in | ios::binary);
        if (!input) {
            cout << argv << ": File not found.  Creating a new file." << endl;
        } else if (!address_book.ParseFromIstream(&input)) {
            cerr << "Failed to parse address book." << endl;
            return -1;
        }
    }
    // Add an address.
    PromptForAddress(address_book.add_people());
    {
        // Write the new address book back to disk.
        fstream output(argv, ios::out | ios::trunc | ios::binary);
        if (!address_book.SerializeToOstream(&output)) {
            cerr << "Failed to write address book." << endl;
            return -1;
        }
    }
    // Optional:  Delete all global objects allocated by libprotobuf.
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}

