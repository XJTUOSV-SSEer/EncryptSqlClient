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
string generateSigmaQuery(string targetTable, string tmpTable ,string whereParams){
    stringstream ss;

    ss << "Select "
    << "enc_query_text_full_table(\'"
    << whereParams << "\',\'"
    << targetTable <<   "\',\'"
    << tmpTable <<  "\'"
    <<                ");"
    ;


    return ss.str();
}
string generateSigmaQueryFromTmpTable(string fromTable, string toTable,string targetTable){
    stringstream ss;

    string select_key = "val";

    ss << "Select " << R"(enc_query_text_full_table(val,')"
                    << targetTable <<  "\',\'"
                    << toTable <<  "\')" << " "
    << "FROM " << fromTable << ";";


    return ss.str();
}

string generateProjetcionQueryFromTmpTable(string fromTable, string params,string tmpTable) {
    stringstream ss;
    ss << "SELECT " << R"(enc_projection_text(val,')"
                    <<  params <<  "\',\'"
                    << tmpTable << "\')" << " "
    << "FROM " << fromTable << ";";

    return ss.str();
}

string generateProjetcionQueryInSingleColumn(string fromTable, string params,string tmpTable) {
    stringstream ss;
    ss << "SELECT " << R"(enc_projection_text(')"
                    <<  params <<  "\',\'"
                    << tmpTable << "\')" << ");";

    return ss.str();
}

string generateSumQuery(string params,string tmpTable) {
    stringstream ss;
    ss << "SELECT " << R"(sum_by_row_to_tmp(')"
                    <<  params <<  "\',\'"
                    << tmpTable << "\')" << ";";

    return ss.str();
}