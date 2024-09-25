//
// Created by 陈志庭 on 24-9-22.
//

#ifndef MySqlClient_H
#define MySqlClient_H
#include <mysql/jdbc.h>
#include<iostream>
#include "../DO/EncKey.h"


using namespace std;

/**
 * Mysql 接口，待完善
 */
class MySqlClient {
private:
    // 驱动
    sql::mysql::MySQL_Driver *driver = nullptr;
    // 连接
    sql::Connection *con = nullptr;
    // 数据库操作执行对象
    sql::Statement *state = nullptr;
    // 结果对象
    sql::ResultSet *result = nullptr;

    string host;
    string username;
    string password;
public:
    MySqlClient(string host,string dbName);

    void setAccount(string username,string password);
    void connect(const string& dbName);
    void disconnect();

    sql::ResultSet *query(const string& query);
    void insertKey(const string& sql_text,EncKey& enc_key);
    void update(string sql_text);




};



#endif //MySqlClient_H
