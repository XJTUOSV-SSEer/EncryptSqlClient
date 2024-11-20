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
    vector<string> params;

public:

    /**
    * SqlPlan 单个计划实体，
    * @param type 类别如 sigma、join、projection 等
    * @param params 每个操作类别对应的参数

*/
    SqlPlan(string type, vector<string> params);
    string getType();
    vector<string> getParams();

};


#endif // ENCRYPTSQLCLIENT_SQLPLAN_H
