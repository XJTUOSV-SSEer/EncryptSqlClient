//
// Created by Zhiti on 2024/11/16.
//

#ifndef ENCRYPTSQLCLIENT_SQLPLAN_H
#define ENCRYPTSQLCLIENT_SQLPLAN_H
#include <iostream>
#include <vector>
using namespace std;
/**
 * 存储SQL计划的类
 */
class SqlPlan {
private:
    string type;
    vector<string> whereParams;
public:
    string getType();
    vector<string> getWhereParams();

};


#endif // ENCRYPTSQLCLIENT_SQLPLAN_H