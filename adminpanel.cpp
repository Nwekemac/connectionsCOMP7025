#include <iostream>
#include <vector>
#include <string>
#include <mariadb/conncpp.hpp>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include <cgicc/Cgicc.h>
#include <cgicc/CgiEnvironment.h>
#include "helperfunctions.h"

using namespace cgicc;
using namespace std;

void listUsers(shared_ptr<sql::Statement> &stmnt) {
  try {
    unique_ptr<sql::ResultSet> res(stmnt->executeQuery("SELECT username, role FROM users"));
    while (res->next()) {
            string sanitised_username = res->getString("username").c_str();
            string sanitised_role = res->getString("role").c_str();
            cout << "<p>Username-: ";
            cout <<  sanitised_username << " >> Role: ";
            cout <<  sanitised_role << "</p>" << endl;
            //cout <<  sanitiseInput(res->getString("username")) << " >> Role: ";
            //cout <<  sanitiseInput(res->getString("role")) << "</p>" << endl;
        }
  } catch (sql::SQLException &e) {
    cout <<"DB error: " << e.what() << endl;
  }

}


int main(int argc, char **argv) {
    cout << HTTPHTMLHeader() << endl;
    cout << html() << endl;
    cout << head() << title("Admin Console") << head() << endl;
    cout << body() << endl;
    cout << h1("Admin Console") << endl;

    Cgicc cgi;


    unique_ptr<sql::Connection> conn = connectDb(); //initiate DB connection by using the connectDB() function created in helperfunctions.h
    shared_ptr<sql::Statement> stmnt(conn->createStatement());


   
    cgicc::form_iterator f_input_username = cgi.getElement("username"); // REF [2]
    cgicc::form_iterator f_input_email = cgi.getElement("email");
    cgicc::form_iterator f_input_password = cgi.getElement("password");
    cgicc::form_iterator f_input_role = cgi.getElement("role");

    const CgiEnvironment& env = cgi.getEnvironment();
    string method = env.getRequestMethod();

    if (method == "POST") {
        //dereferencing to declear usable variables in c++
    string username = **f_input_username;
    string email = **f_input_email;
    string password = **f_input_password;
    string role = **f_input_role;
    password = hashPw(password); // hash the passowrd
    
      if (!isValidUsername(username)) {
        cout << p("Invalid Username. Username should contain only letters and numbers and be 3 to 20 characters long") << endl;
      } else {
        try {
        //Prepared Statement to take ADD user to database;
        shared_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement (
          "INSERT into users (username, role, email, password_hash) VALUES (?, ?, ?, ?)"
        )
        );
        pstmt->setString(1, username);
        pstmt->setString(2, role);
        pstmt->setString(3, email);
        pstmt->setString(4, password);
        pstmt->executeUpdate();
        } catch (sql::SQLException &e) {
        cout <<"DB error: " << e.what() << endl;
        }

    }


    }
    
    
    


    //CREATE_USER FORM
    cout << "<form method='POST'>" << endl;
    cout << "Username: <input type='text' name='username'><br><br>" << endl;
    cout << "Email: <input type='email' name='email'><br><br>" << endl;
    cout << "Password: <input type='password' name='password'><br><br>" << endl;
    cout << "Role: <select name='role'>" << endl;
    cout << "<option value='client'>Client</option>" << endl;
    cout << "<option value='provider'>Provider</option>" << endl;
    cout << "<option value='admin'>Admin</option>" << endl;
    cout << "</select><br><br>" << endl;
    cout << "<input type='submit' value='Create User'>" << endl;
    cout << "</form>" << endl;

  // Test for sanitiseInput
    cout << sanitiseInput("AZ$$<test>") << endl;
    
  ///List users
    cout << hr();
    listUsers(stmnt);

    //TEST for HASH Function
    cout << hashPw("testing123") << endl;

    cout << body() << html() << endl;
    return 0;
}




