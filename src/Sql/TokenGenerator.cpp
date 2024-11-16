//
// Created by Zhiti on 2024/11/16.
//
#include "TokenGenerator.h"
#include <sstream>
using namespace std;
string generateSigmaQuery(string targetTable, vector<string> texts,bool return_key){
    stringstream ss;


    string select_key;
    string predicate;
    string params;


    if(return_key)select_key = "encKey";
    else select_key = "encValue";

    if(texts.size() == 1) {
        predicate = "=";
        params = texts[0];
    } else {
        predicate = "in";
        ss << "(" << texts[0];
        for(int i=1;i<texts.size();i++){
            ss << "," << texts[i];
        }
        ss << ")";
        params = ss.str();
        ss.str("");
    }

    ss << "SELECT " << select_key << " " << "FROM " << targetTable << " " << "WHERE encValue " << predicate
       << " " << params << ";";

    return ss.str();
}