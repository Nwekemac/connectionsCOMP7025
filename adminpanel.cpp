#include <iostream>
#include <vector>
#include <mariadb/conncpp.hpp>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>


using namespace cgicc;
using namespace std;

int main(int argc, char **argv) {
    cout << HTTPHTMLHeader() << endl;
    cout << html() << endl;
    cout << head() << title("Admin Console") << head() << endl;
    cout << body() << endl;
    cout << h1("Admin Console") << endl;

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


    cout << body() << html() << endl;
    return 0;
}




