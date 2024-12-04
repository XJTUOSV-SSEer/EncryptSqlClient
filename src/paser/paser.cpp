#include <algorithm>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include "../EncryptManager.h"
#include "../dataObject/dataObject.h"
using namespace std;

// 判断是否为普通查询
bool isSelectType(const string& sqlQuery) {
    regex selectRegex(R"(SELECT\s+(.*?)\s+FROM)", regex::icase);
    smatch match;

    if (regex_search(sqlQuery, match, selectRegex)) {
        string selectClause = match[1];

        // 检查是否包含聚合函数，如 SUM
        regex sumRegex(R"(SUM\()", regex::icase);
        if (!regex_search(selectClause, sumRegex)) {
            cout << "这是普通查询" << endl;
            return true; // 没有 SUM，则是普通 SELECT
        }
    }
    return false; // 未找到 SELECT 或包含聚合函数
}

// 判断是否为聚合查询（如 SUM）
bool isAggregationType(const string& sqlQuery) {
    regex selectRegex(R"(SELECT\s+(.*?)\s+FROM)", regex::icase);
    smatch match;

    if (regex_search(sqlQuery, match, selectRegex)) {
        string selectClause = match[1];

        // 检查是否包含 SUM
        regex sumRegex(R"(SUM\()", regex::icase);
        if (regex_search(selectClause, sumRegex)) {
            cout << "这是包含聚合函数的查询" << endl;
            return true; // 包含 SUM，则是聚合查询
        }
    }
    return false; // 未找到 SELECT 或不包含聚合函数
}

// 解析 WHERE 子句中的查询值，并返回它在二维数组中的列索引
int parseWhereConditionColIndex(const string& sqlQuery, const vector<string>& table) {
    // 修改正则表达式以匹配带单引号的值
    regex whereRegex(R"(WHERE\s+(\w+)\s*=\s*'(\w+)')", regex::icase);
    smatch match;

    if (regex_search(sqlQuery, match, whereRegex)) {
        string field = match[1];    // 提取字段名
        string value = match[2];    // 提取字段值（去除单引号）

        // 查找字段名在表头中的列索引
        int colIndex = -1;
        for (int i = 0; i < table.size(); ++i) {
            if (table[i] == field) {
                colIndex = i; // 找到字段名所在列的索引
                return i;
            }
        }

        // 如果未找到字段名，返回 -1
        if (colIndex == -1) {
            return -1;
        }

    }

    return -1; // 未找到匹配的列，返回 -1
}

string parseWhereCondition(const string& sqlQuery) {
    // 修改正则表达式以匹配带单引号的值
    regex whereRegex(R"(WHERE\s+(\w+)\s*=\s*'(\w+)')", regex::icase);
    smatch match;

    if (regex_search(sqlQuery, match, whereRegex)) {
        string field = match[1];    // 提取字段名
        string value = match[2];    // 提取字段值（去除单引号）
        return value;
    }
    return ""; // 未找到匹配的列，返回 -1
}


// 解析普通 SELECT 查询的属性并返回所在的列索引
int parseSelectAttributeColIndex(const string& sqlQuery, const vector<string>& table) {
    regex selectRegex(R"(SELECT\s+(\w+)\s+FROM)", regex::icase);
    smatch match;

    if (regex_search(sqlQuery, match, selectRegex)) {
        string attribute = match[1]; // 提取 SELECT 和 FROM 之间的字段

        // 匹配字段名到表头，返回列索引
        for (int i = 0; i < table.size(); ++i) {
            if (table[i] == attribute) {
                return i; // 返回列索引
            }
        }
    }
    return -1; // 未找到，返回 -1
}

// 解析 SUM 聚合查询的属性并返回所在的列索引
int parseSumAttributeColIndex(const string& sqlQuery, const vector<string>& table) {
    regex sumRegex(R"(SUM\((\w+)\))", regex::icase);
    smatch match;

    if (regex_search(sqlQuery, match, sumRegex)) {
        string attribute = match[1]; // 提取 SUM 中的字段

        // 匹配字段名到表头，返回列索引
        for (int i = 0; i < table.size(); ++i) {
            if (table[i] == attribute) {
                return i; // 返回列索引
            }
        }
    }
    return -1; // 未找到，返回 -1
}


//vector<SqlPlan> parseSql(string sql,map<string,Table> tables)

vector<SqlPlan> parseSql(string sql,Table tableinfo)
{
    stringstream ss;
    //Table tableinfo;
    //tableinfo["student"].get_columns();
    //for(const auto& it:tables) {
    //    tableinfo = it.second;
    //}
    vector<string> col_name = tableinfo.get_columns();
    vector<string> col_types = tableinfo.get_columns_type();
    //测试语句
    // string s = "SELECT id FROM student WHERE name = 'Bob'";

    vector<SqlPlan> plans;
    vector<string> res_param;
    if(isAggregationType(sql))     //如果是包含聚合函数的查询
    {
        //cout << parseSumAttributeColIndex(sql,col_name) << endl;
        int colIndex = parseSumAttributeColIndex(sql, col_name);

        res_param.push_back(col_types[colIndex]);
        ss << tableinfo.get_name()<< "," << colIndex;

        string p3 = ss.str();
        vector<string> actual_p3 = {prfFunctionReturnString(p3,true)};
        ss.str("");
        SqlPlan pl3("sum",actual_p3);
        plans.push_back(pl3);
    }
    else if(isSelectType(sql))        //如果是简单查询
    {
        int colIndex1 = parseWhereConditionColIndex(sql, col_name);
        //cout << "所在的列是" << parseWhereConditionColIndex(sql,col_name) << endl;           //where子句后的内容所在的列
        ss << tableinfo.get_name() <<","
           << colIndex1 << ","
           << parseWhereCondition(sql);
        string p1 = ss.str();
        vector<string> actual_p1 = {prfFunctionReturnString(p1, true)};
        ss.str("");
        SqlPlan pl1("select",actual_p1);
        plans.push_back(pl1);

        //cout << "所在的列是" << parseSelectAttributeColIndex(sql,col_name) << endl;
        int colIndex2 = parseSelectAttributeColIndex(sql, col_name);//select后的内容所在的列
        res_param.push_back(col_types[colIndex2]);
        ss << colIndex2;
        string p2 = ss.str();
        vector<string> actual_p2 = {p2};
        ss.str("");
        SqlPlan pl2("projection",actual_p2);
        plans.push_back(pl2);

    }

    SqlPlan resPlan("result",res_param);
    plans.push_back(resPlan);
    return plans;

}

