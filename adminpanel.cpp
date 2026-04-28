#include <iostream>
#include <vector>
#include <string>
#include <mariadb/conncpp.hpp>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
#include <cgicc/Cgicc.h>
#include <cgicc/CgiEnvironment.h>
//Includes for hashing function
#include <openssl/evp.h>
#include <iomanip>
#include <sstream>


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
//HASHING function
/**The function below takes a password as a string and returns
the output (hashed with SHA-256
This function was created by refactoring a program found online 
demonstrating hashing with OPENSSL using C
More details provided in [REF 3] in report**/
string hashPw(const string& password) {
    EVP_MD_CTX* context = EVP_MD_CTX_create();
    EVP_DigestInit(context, EVP_sha256());
    EVP_DigestUpdate(context, password.c_str(), password.size());

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_length = 0;
    EVP_DigestFinal(context, hash, &hash_length);
    EVP_MD_CTX_free(context);

    // Convert hash bytes to readable hex string
    stringstream output_hash;
    for (unsigned int i = 0; i < hash_length; i++) {
        output_hash << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return output_hash.str();
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

    //TEST for HASH Function
    cout << hashPw("testing12345") << endl;

    cout << body() << html() << endl;
    return 0;
}




