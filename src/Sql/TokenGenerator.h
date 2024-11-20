//
// Created by Zhiti on 2024/11/16.
//

#ifndef ENCRYPTSQLCLIENT_TOKENGENERATOR_H
#define ENCRYPTSQLCLIENT_TOKENGENERATOR_H
#include "string"
#include "vector"
using namespace std;
string generateSigmaQueryArc(string targetTable, vector<string> texts,bool return_key = true);
/**
* 生成一个对加密数据表的 Select 查询
* @param targetTable 操作的加密表
* @param tmpTable 暂存的加密表，用于向下一次操作传递参数
* @param whereParams 等值链接的数据
*/
string generateSigmaQuery(string targetTable, string tmpTable ,string whereParams);
/**
* 生成一个对加密数据表的 Select 查询，用于中间操作查询
* @param fromTable 上一个查询结果的暂存表
* @param toTable 暂存的加密表，用于向下一次操作传递参数
* @param targetTable 执行查询操作的表
*/
string generateSigmaQueryFromTmpTable(string fromTable, string toTable,string targetTable);

string generateProjetcionQueryFromTmpTable(string fromTable, string params,string tmpTable);
#
#endif // ENCRYPTSQLCLIENT_TOKENGENERATOR_H
