#include <iostream>
#include <vector>
#include <string>
#include <mariadb/conncpp.hpp>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include <cgicc/Cgicc.h>
#include <cgicc/CgiEnvironment.h>
#include "helperfunctions.h"
#include <cgicc/HTTPRedirectHeader.h> //to handle redirects
using namespace cgicc;
using namespace std;


int main(int argc, char **argv) {


    Cgicc cgi;


    unique_ptr<sql::Connection> conn = connectDb(); //initiate DB connection by using the connectDB() function created in helperfunctions.h
    shared_ptr<sql::Statement> stmnt(conn->createStatement());


    const CgiEnvironment& env = cgi.getEnvironment();
    string method = env.getRequestMethod();

    if (method == "POST") {
      cgicc::form_iterator f_input_username = cgi.getElement("username"); // REF [2]
      cgicc::form_iterator f_input_password = cgi.getElement("password");
      //dereferencing to declear usable variables in c++
      string username = **f_input_username;
      string password = **f_input_password;
      password = hashPw(password); // hash the passowrd
    
    
      try {
        //Confirm if username & passowrd correct
        shared_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(
          "SELECT username, role FROM users where username = ? AND password_hash = ?"
        ));
        pstmt->setString(1, username);
        pstmt->setString(2, password);
        unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
          cout << HTTPRedirectHeader("dashboard.cgi") << endl;
        }
        else {
          cout << p("Incorrect pass or username") << endl;
        }
      } catch (sql::SQLException &e) {
        cout <<"DB error: " << e.what() << endl;
      }


    }
    
    cout << HTTPHTMLHeader() << endl;
    cout << html() << endl;
    cout << head() << title("Sign in") << head() << endl;
    cout << body() << endl;
    cout << h1("Sign in") << endl;    
    


    //LOGIN FORM
    cout << "<form method='POST'>" << endl;
    cout << "Username: <input type='text' name='username'><br><br>" << endl;
    cout << "Password: <input type='password' name='password'><br><br>" << endl;
    cout << "<input type='submit' value='Log in'>" << endl;
    cout << "</form>" << endl;

    cout << body() << html() << endl;
    return 0;
}

