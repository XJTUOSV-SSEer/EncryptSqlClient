//
// Created by 陈志庭 on 24-11-26.
//


#ifndef PASER_H
#define PASER_H
#include "../dataObject/dataObject.h"


vector<SqlPlan> parseSql(string sql,map<string,TableInfo> tables);

vector<SqlPlan> parseSql(string sql,Table tableinfo);
#endif //PASER_H
