//
// Created by Zhiti on 2024/11/16.
//
#include "TokenGenerator.h"
#include <sstream>
using namespace std;
string generateSigmaQueryArc(string targetTable, vector<string> whereParams,bool return_key){
    stringstream ss;


    string select_key;
    string predicate;
    string params;


    if(return_key)select_key = "enc_key";
    else select_key = "enc_value";

    if(whereParams.size() == 1) {
        predicate = "=";
        params = whereParams[0];
    } else {
        predicate = "in";
        ss << "(" << whereParams[0];
        for(int i=1;i<whereParams.size();i++){
            ss << "," << whereParams[i];
        }
        ss << ")";
        params = ss.str();
        ss.str("");
    }

    ss << "SELECT " << select_key << " "
    << "FROM " << targetTable << " "
    << "WHERE enc_value " << predicate<< " " << params << ";";

    return ss.str();
}
string generateSigmaQuery(string fromTable, string tmpTable ,string whereParams){
    stringstream ss;

    ss << "INSERT "
    << "enc_query_text(enc_value) "
    << "INTO " << tmpTable << " "
    << "FROM " << fromTable << " "
    << "WHERE enc_key "  << "= " << whereParams << ";";

    return ss.str();
}
string generateSigmaQueryFromTmpTable(string fromTable, string tmpTable){
    stringstream ss;

    string select_key = "val";

    ss << "INSERT " << "enc_query_text(val)" << " "
    << "INTO " << tmpTable << " "
    << "FROM " << fromTable << ";";


    return ss.str();
}