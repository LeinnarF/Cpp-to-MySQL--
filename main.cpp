#include <iostream>
#include <windows.h>
#include <mysql.h>
#include <sstream>
#include <stdio.h>
#include <iomanip>

using namespace std;
//Global
MYSQL* conn;
const char* database   = "test69";          //change this according to your database name
const char* ip_address = "192.168.254.100"; //change this according to you IP address
const char* username;                       //accessed via login
const char* password;                       //accessed via login

char usernameBuffer[100];
char passwordBuffer[100];

// Declaration
bool Login();
void Register();
void insertData();
void viewData();
void editData();
void removeData();
void birthDate(int& month, int& day, int& year);
int computeAge(int birthYear, int birthMonth, int birthDay);

int main(){

    MYSQL* conn;
    bool loop1 = true;
    int choice1, choice2;

    while(loop1)
    {
        system("cls");
        bool loop2 = true;
        while(loop2)
        {
            system("cls");
            cout << "+================================+" << endl;
            cout << "|                                |" << endl;
            cout << "|    STUDENT DATABASE MANAGER    |" << endl;
            cout << "|                                |" << endl;
            cout << "+================================+" << endl;
            cout << endl;
            cout << "Select operation         " << endl;
            cout << "[1] Log in              " << endl;;
            cout << "[2] Register            "<< endl;;
            cout << "[0] Exit                " << endl;
            cout << "\n > "; cin >> choice1;

            switch(choice1)
            {
                case 1:
                    if(Login()==true)
                        loop2 = false;
                    break;

                case 2:
                    Register();
                    break;

                default:
                    return 0;
            }
        }
        bool loop3 = true;
        while (loop3)
        {
            system("cls");
            cout << "+------------------------------------------+" << endl;
            cout << "|       STUDENT DATABASE MANAGER           |" << endl;
            cout << "+------------------------------------------+" << endl;
            cout << endl;
            cout << "[1] Register Student's Infomation     " << endl;
            cout << "[2] Update Student's Infomation       " << endl;
            cout << "[3] Delete Student's Infomation       " << endl;
            cout << "[4] View Table                        " << endl;
            cout << "[0] Log out                           " << endl;
            cout << "\n > "; cin >> choice2;
            cout << "\n";

            switch(choice2)
            {
                case 1:
                    insertData();
                    break;

                case 2:
                    editData();
                    break;

                case 3:
                    removeData();
                    break;

                case 4:
                    viewData();
                    break;

                default:
                    loop3 = false;
                    break;
            }
        }
    }
}

bool Login(){
    system("cls");
    cout << "LOG IN\n" << endl;
    cout << "username: ";
    cin >> usernameBuffer;
    username = usernameBuffer;

    cout << "password: ";
    cin >> passwordBuffer;
    password = passwordBuffer;

    conn = mysql_init(0);
    conn = mysql_real_connect(conn,ip_address,username, password, database,0,NULL,0);

    if(conn){
        cout << "\nDatabase Connected Successfully!\n" << endl;
        Sleep(1500);
        return true;
    }
    else{
         cout << "Failed to Connect Database.\n" << endl;
         Sleep(2000);
        return false;
    }
}

void Register(){
    system("cls");
    string reg_username, reg_password;
    cout << "REGISTER\n" << endl;
    cout << "username: "; cin >> reg_username;
    cout << "password: "; cin >> reg_password;

    int qstate = 0;
    stringstream ss;
    ss << "CREATE USER '"<<reg_username<<"'@'%' IDENTIFIED BY '"<<reg_password<<"';";
    string query = ss.str();
    const char* q = query.c_str();
    qstate = mysql_query(conn, q);

    if(qstate == 0){
        cout << "Registered" << endl;

        ss.str("");
        ss << "GRANT ALL PRIVILEGES ON *.* TO '"<<reg_username<< "'@'%';";
        query = ss.str();
        q = query.c_str();
        qstate = mysql_query(conn,q);
    }
}

void insertData(){
    system("cls");
    conn = mysql_init(0);
    conn = mysql_real_connect(conn,ip_address,username, password, database,0,NULL,0);

    int qstate = 0;
    stringstream ss;
    string f_name, l_name, m_name, stud_id, contact, bday;
    int month, day, year;
    cin.ignore();

    system("cls");
    cout << "Student's First Name : ";
    getline(cin, f_name);
    cout << "Student's Last Name  : ";
    getline(cin, l_name);
    cout << "Student's Middle Name: ";
    getline(cin,m_name);

    system("cls");
    cout << "Contact number: ";
    getline(cin, contact);

    system("cls");
    birthDate(month, day, year);
    stringstream bss;
    bss << year << '-' << month << '-' << day;
    bday = bss.str();

    //cout << "Age: ";
    int age = computeAge(year, month, day);

    ss << "INSERT INTO student(first_name,last_name,middle_name,birth_date,age,contact) VALUES ('"<<f_name<<"', '"<<l_name<<"', '"<<m_name<<"', '"<<bday<<"', '"<<age<<"', '"<<contact<<"')";

    string query = ss.str();
    const char* q = query.c_str();
    qstate = mysql_query(conn, q);
}

void viewData(){
    system("cls");
    MYSQL_RES* result;
    MYSQL_ROW row;
    int qstate = 0;

    const char* q = "SELECT * FROM student";
    qstate = mysql_query(conn,q);
    result = mysql_store_result(conn);
    cout << "+----+----------------------+-----------------+-----------------+------------+-----+--------------+\n";
    cout << "| ID |     First Name       |    Last Name    |   Middle Name   | Birth Date | Age | Contact      |\n";
    cout << "+----+----------------------+-----------------+-----------------+------------+-----+--------------+\n";
    while ((row = mysql_fetch_row(result))) {
        cout << "| " << std::setw(2) << row[0] << " | " << std::setw(20) << row[1] << " | " << std::setw(15) << row[2]
            << " | " << std::setw(15) << row[3] << " | " << std::setw(10) << row[4] << " | " << std::setw(3) << row[5]
            << " | " << std::setw(12) << row[6] << " |\n";
    }
 cout <<    "+----+----------------------+-----------------+-----------------+------------+-----+--------------+\n";
    //mysql_free_result(result);
    system("pause");
}

void editData(){
    system("cls");
    conn = mysql_init(0);
    conn = mysql_real_connect(conn,ip_address,username, password, database,0,NULL,0);
    int qstate = 0;

    bool loop = true;
    while(loop)
    {
    string index;
    int option;

    system("cls");
    cout << "+-------------------+" << endl;
    cout << "|      UPDATE       |" << endl;
    cout << "+-------------------+" << endl;
    cout << "\n";
    cout << "[1] View table\n[2] Update information\n[0] Back" << endl;
    cout << "\n> ";cin >> option;

    if(option == 0 || option >=3)
    loop = false;

    switch(option){
    case 1:
        viewData();
        break;
    case 2:
        int choice1,choice2;
        string column, new_val, id;
        cout << "   Select column   " << endl;
        cout << "-------------------" << endl;
        cout << "[1] First name" << endl;
        cout << "[2] Last name"  << endl;
        cout << "[3] Middle name"<< endl;
        cout << "[4] Contact"    << endl;
        cout << "[5] Birth date" << endl;
        cout << "\n> "; cin >> choice1;
        if(choice1 >= 6 || choice1 == 0){
            cout << "Column does not Exist " << endl;
            Sleep(1500);
            break;
        }
        cout << "Select ID: "; cin >> id;

            {
                stringstream checkQuery;
                checkQuery << "SELECT * FROM student WHERE student_id = '" << id << "';";
                string checkQueryString = checkQuery.str();
                const char* checkQueryChar = checkQueryString.c_str();

                if (mysql_query(conn, checkQueryChar) != 0) {
                    cerr << "\nError checking student_id existence: " << mysql_error(conn) << endl;
                    Sleep(2000);
                    break;
                }

                MYSQL_RES* result = mysql_store_result(conn);
                if (mysql_num_rows(result) == 0) {
                    cout << "\nStudent ID does not exists.\n" << endl;
                    Sleep(1500);
                    break;
                }

                mysql_free_result(result);
            }

        switch(choice1){
        case 1:
            column = "first_name";
            cout << "New value: ";cin >> new_val;
            break;
        case 2:
            column = "last_name";
            cout << "New value: ";cin >> new_val;
            break;
        case 3:
            column = "middle_name";
            cout << "New value: ";cin >> new_val;
            break;
        case 4:
            column = "contact";
            cout << "New value: ";cin >> new_val;
            break;
        case 5:
            int year, day, month;
            string bday;
            birthDate(month, day, year);
            stringstream bss;
            bss << year << '-' << month << '-' << day;
            bday = bss.str();
            column = "birth_date";
            new_val = bday;

            stringstream q_age;
            string column2;
            int new_age = computeAge(year, month, day);

            q_age << "UPDATE student SET age = '"<<new_age<<"' WHERE student_id = '"<<id<<"' ;";
            string query2 = q_age.str();
            const char* q2 = query2.c_str();
            qstate = mysql_query(conn, q2);
            break;
        }

        int qstate = 0;
        stringstream ss;
        ss << "UPDATE student SET "<<column<<" = '"<< new_val <<"' WHERE student_id = '"<<id<<"' ;";
        string query = ss.str();
        const char* q = query.c_str();
        qstate = mysql_query(conn, q);
        break;
        }
    }
}

void removeData(){
    system("cls");
    conn = mysql_init(0);
    conn = mysql_real_connect(conn,ip_address,username, password, database,0,NULL,0);

    bool loop = true;

while(loop)
    {
    loop = true;
    int option;
    string id;

    system("cls");
    cout << "+--------------------+" << endl;
    cout << "|       REMOVE       |" << endl;
    cout << "+--------------------+" << endl;
    cout << "\n";
    cout << "[1] View table\n[2] Remove Information\n[0] Back" << endl;
    cout << "\n> "; cin >> option;

    if(option == 0 || option >=3)
        loop = false;

    switch(option){
    case 1:
        viewData();
        break;

    case 2:
        cout << "Select ID: "; cin >> id;

        int qstate = 0;
        stringstream ss;

        ss << "DELETE FROM student WHERE student_id = '"<<id<<"';";

                    {
                stringstream checkQuery;
                checkQuery << "SELECT * FROM student WHERE student_id = '" << id << "';";
                string checkQueryString = checkQuery.str();
                const char* checkQueryChar = checkQueryString.c_str();

                if (mysql_query(conn, checkQueryChar) != 0) {
                    cerr << "\nError checking student_id existence: " << mysql_error(conn) << endl;
                    Sleep(2000);
                    break;
                }

                MYSQL_RES* result = mysql_store_result(conn);
                if (mysql_num_rows(result) == 0) {
                    cout << "\nStudent ID does not exists.\n" << endl;
                    Sleep(1500);
                    break;
                }

                mysql_free_result(result);
            }

        string query = ss.str();
        const char* q = query.c_str();
        qstate = mysql_query(conn,q);
        break;
        }
    }
}

void birthDate(int& month, int& day, int& year){
        cout << "          BIRTH DATE          " << endl;
    system("cls");
    while(month < 1 || month > 12 || day < 1 || day > 31 || year < 1900 || year > 2100)
    {
        cout << "          Birth Date          "<< endl;
        cout << "------------------------------"<< endl;
        cout << "[1] January      [7] July"     << endl;
        cout << "[2] February     [8] August"   << endl;
        cout << "[3] March        [9] September"<< endl;
        cout << "[4] April       [10] October"  << endl;
        cout << "[5] May         [13] November" << endl;
        cout << "[7] June        [12] December" << endl;
        cout << "------------------------------"<< endl;
        cout << endl;
        cout << "Month : "; cin >> month;
        cout << "Day   : "; cin >> day;
        cout << "Year  : "; cin >> year;
        cout << endl;

        if (month < 1 || month > 12 || day < 1 || day > 31 || year < 1900 || year > 2100) {
            cerr << "Invalid date. Please enter a valid date.\n" << endl;
        }
    }
}

int computeAge(int birthYear, int birthMonth, int birthDay){

    time_t now = time(0);
    tm* ltm = localtime(&now);

    int currentYear = 1900 + ltm->tm_year;
    int currentMonth = 1 + ltm->tm_mon;
    int currentDay = ltm->tm_mday;

    int age = currentYear - birthYear;

    if (currentMonth < birthMonth || (currentMonth == birthMonth && currentDay < birthDay)) {
        age--;
    }

    return age;
}
