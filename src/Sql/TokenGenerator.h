//
// Created by Zhiti on 2024/11/16.
//

#ifndef ENCRYPTSQLCLIENT_TOKENGENERATOR_H
#define ENCRYPTSQLCLIENT_TOKENGENERATOR_H
#include "string"
#include "vector"
using namespace std;
string generateSigmaQueryArc(string targetTable, vector<string> texts,bool return_key = true);
string generateSigmaQuery(string fromTable, string tmpTable ,string whereParams);
string generateSigmaQueryFromTmpTable(string fromTable, string tmpTable);
#
#endif // ENCRYPTSQLCLIENT_TOKENGENERATOR_H
