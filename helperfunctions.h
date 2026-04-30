#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <string>
#include <sstream>
#include <iomanip>
#include <openssl/evp.h>
#include <mariadb/conncpp.hpp>

using namespace std;


//Function for DB Connection 
unique_ptr<sql::Connection> connectDb() {
    sql::Driver* driver = sql::mariadb::get_driver_instance();
    sql::SQLString url("jdbc:mariadb://localhost:3306/comp7025");
    sql::Properties properties({{"user", "db_user"}, {"password", "db_user_password"}});
    return unique_ptr<sql::Connection>(driver->connect(url, properties));
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

#endif