//
// Created by Zhiti on 2024/11/16.
//

#ifndef ENCRYPTSQLCLIENT_TOKENGENERATOR_H
#define ENCRYPTSQLCLIENT_TOKENGENERATOR_H
#include "string"
#include "vector"
using namespace std;
string generateSigmaQuery(string targetTable, vector<string> texts,bool return_key = true);
string generateJoinToken(string targetTable, vector<string> texts,bool return_key = true);
string generatePiToken(string targetTable, vector<string> texts,bool return_key = true);
#
#endif // ENCRYPTSQLCLIENT_TOKENGENERATOR_H
