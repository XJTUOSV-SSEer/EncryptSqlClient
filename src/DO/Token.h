//
// Created by Zhiti on 2024/11/16.
//

#ifndef ENCRYPTSQLCLIENT_TOKEN_H
#define ENCRYPTSQLCLIENT_TOKEN_H
#include <vector>
#include <string>
using namespace std;

class Token {
private:
    /**
     * 参数化查询语句
     */
    string sqlQuery;
    /**
     * 查询参数
     */
    vector<string> paramValues;
    /**
     * 参数长度，二进制数据需要，否则可以为0。
     */
    vector<int> paramLengths;
    /**
     * 参数类型，文本类型为0,二进制类型为1。
     */
    vector<int> paramFormats;
public:
    Token(string sqlQuery,vector<string> paramValues,vector<int> paramLengths,vector<int>paramFormats);
    const char* getQuery();
    const char** getparamValues();
    int* getparamLengths();
    int* getparamFormats();


};


#endif // ENCRYPTSQLCLIENT_TOKEN_H
