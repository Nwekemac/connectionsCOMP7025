#include <iostream>
#include <vector>
#include <string>
#include <mariadb/conncpp.hpp>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include <cgicc/Cgicc.h>
#include <cgicc/CgiEnvironment.h>


using namespace cgicc;
using namespace std;

void listUsers(shared_ptr<sql::Statement> &stmnt) {
  try {
    //shared_ptr<sql::Statement> select_stmt(conn->createStatement());
    //unique_ptr<sql::ResultSet> res(select_stmt->executeQuery("SELECT username, role FROM users"));
    unique_ptr<sql::ResultSet> res(stmnt->executeQuery("SELECT username, role FROM users"));
    while (res->next()) {
            cout << "<p>Username :";
            cout <<  res->getString("username") << " >> ";
            cout <<  res->getString("role") << "</p>" << endl;
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

    //DB_CONNECTION
    sql::Driver* driver = sql::mariadb::get_driver_instance();
    sql::SQLString url("jdbc:mariadb://localhost:3306/comp7025"); 
    sql::Properties  properties({
        {"user", "db_user"},
        {"password", "db_user_password"}
    });
    unique_ptr<sql::Connection> conn(driver->connect(url, properties));
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


  ///List users
    cout << hr();
    listUsers(stmnt);

    cout << body() << html() << endl;
    return 0;
}




