//
// Created by 陈志庭 on 24-9-22.
//
#include "MySqlClient.h"
#include "../EncryptTools/EncryptUtil.h"
#include <mysql/jdbc.h>
#include <utility>

#include "../DO/EncKey.h"
using namespace std;


MySqlClient::MySqlClient(string host,string dbName) {
    this->driver = sql::mysql::get_mysql_driver_instance();
    this->host = move(host);
}
void MySqlClient::setAccount(string username,string password) {
    this->username = move(username);
    this->password = move(password);
}
void MySqlClient::connect(const string& dbName) {
    this->con = this->driver->connect(this->host,this->username,this->password);
}

sql::ResultSet *MySqlClient::query(const string& query) {
    sql::Statement *stmt;
    sql::ResultSet *res;

    stmt = this->con->createStatement();
    res = stmt->executeQuery(query);

    delete stmt;
    return res;
}
void MySqlClient::insertKey(const string& sql_text,EncKey& enc_key) {
    sql::PreparedStatement *pstmt = this->con->prepareStatement(sql_text);



    pstmt->setString(1,enc_key.index_str);
    pstmt->setString(2,enc_key.MMType);

    pstmt->setString(3,enc_key.key);
    pstmt->setString(4,enc_key.iv);
}




void MySqlClient::disconnect() {
    delete this->con;
}
