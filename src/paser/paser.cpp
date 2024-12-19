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

//判断是否是连接查询
bool isJoinType(const string &sqlQuery) {
    // 转换为小写，便于匹配（假设 SQL 不区分大小写）
    string lowerQuery = sqlQuery;
    transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    // 正则表达式匹配显式连接（JOIN 语句）
    regex explicitJoinRegex(R"(select\s+.*\s+from\s+.*\s+join\s+.*\s+on\s+.*)", regex::icase);

    // 正则表达式匹配隐式连接（WHERE 子句中涉及多个表）
    regex implicitJoinRegex(R"(select\s+.*\s+from\s+[^,]+,[^,]+\s+where\s+.*\.\w+\s*=\s*.*\.\w+)", regex::icase);

    // 使用正则表达式匹配显式或隐式连接
    if (regex_search(lowerQuery, explicitJoinRegex) || regex_search(lowerQuery, implicitJoinRegex)) {
        return true; // 是连接查询
    }
    return false; // 不是连接查询
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
std::vector<std::string> extractSingleQuoteContent(const std::string& input) {
    std::vector<std::string> results;
    std::regex pattern(R"('([^'\\]*(\\.[^'\\]*)*)')");  // 正则表达式匹配单引号内的内容
    std::smatch matches;

    auto it = input.begin();
    while (std::regex_search(it, input.end(), matches, pattern)) {
        results.push_back(matches[1].str());  // 提取单引号内的内容
        it = matches.suffix().first;  // 更新迭代器位置，继续搜索
    }

    return results;
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
//以下是多表操作函数
// 多表表名解析：解析 SQL 查询中的表名，返回表名列表
vector<string> parseTables(const string& sqlQuery) {
    vector<string> tableNames; // 存储表名的结果
    set<string> tableSet;      // 用于去重

    // 正则表达式 - 提取 FROM 子句后的主表及其别名
    regex fromRegex(R"(\bFROM\s+([a-zA-Z0-9_]+)(?:\s+([a-zA-Z0-9_]+))?)", regex::icase);
    smatch match;
    if (regex_search(sqlQuery, match, fromRegex)) {
        string tableName = match[1];
        tableSet.insert(tableName); // 主表名
    }

    // 正则表达式 - 提取 JOIN 子句中的表及其别名
    regex joinRegex(R"((?:JOIN|INNER\s+JOIN|LEFT\s+JOIN|RIGHT\s+JOIN|FULL\s+JOIN)\s+([a-zA-Z0-9_]+)(?:\s+([a-zA-Z0-9_]+))?)", regex::icase);
    string query = sqlQuery;
    while (regex_search(query, match, joinRegex)) {
        string tableName = match[1];
        if (tableSet.find(tableName) == tableSet.end()) { // 如果表名未记录
            tableSet.insert(tableName);
        }
        query = match.suffix().str(); // 继续解析
    }

    // 正则表达式 - 提取 WHERE 子句中的隐式表名
    regex whereRegex(R"(([a-zA-Z0-9_]+)\.[a-zA-Z0-9_]+\s*=)", regex::icase);
    query = sqlQuery;
    while (regex_search(query, match, whereRegex)) {
        string tableName = match[1];
        if (tableSet.find(tableName) == tableSet.end()) {
            tableSet.insert(tableName);
        }
        query = match.suffix().str(); // 继续解析
    }

    // 将 set 中的表名存入 vector
    tableNames.assign(tableSet.begin(), tableSet.end());
    return tableNames;
}

// 多表投影函数：解析 SQL 查询中的 SELECT 子句属性名
vector<string> parseSelectAttributes(string& sqlQuery) {
    vector<string> attributes; // 用于存储属性名

    // 正则表达式 - 提取 SELECT 和 FROM 之间的部分
    regex selectRegex(R"(\bSELECT\s+(.*?)\s+FROM\b)", regex::icase);
    smatch match;

    // 如果匹配成功
    if (regex_search(sqlQuery, match, selectRegex)) {
        string selectClause = match[1]; // 获取 SELECT 和 FROM 之间的部分

        // 分割属性名，以逗号为分隔符
        regex attributeRegex(R"(\s*([^,\s]+)\s*(?:,|$))");
        string::const_iterator searchStart(selectClause.cbegin());
        smatch attrMatch;

        while (regex_search(searchStart, selectClause.cend(), attrMatch, attributeRegex)) {
            attributes.push_back(attrMatch[1]); // 保存每个属性名
            searchStart = attrMatch.suffix().first; // 移动到下一部分
        }
    }

    return attributes;
}

int findColIndex(vector<string> col_names,string col) {
    for(int i = 0; i < col_names.size(); ++i) {
        if(col_names[i] == col) {
            return i;
        }
    }
}

// 多表条件函数：解析 WHERE 子句中的等号表达式
vector<pair<string, string>> parseWhereConditions(const string& sqlQuery) {
    vector<pair<string, string>> conditions; // 存储等号两侧的属性名

    // 正则表达式 - 匹配 WHERE 子句及其后面的条件
    regex whereRegex(R"(\bWHERE\s+(.*))", regex::icase); // 匹配 WHERE 后的条件部分
    smatch match;

    if (regex_search(sqlQuery, match, whereRegex)) {
        string whereClause = match[1]; // 获取 WHERE 子句后的部分

        // 正则表达式 - 提取等号两侧的内容
        regex conditionRegex(R"((\S+)\s*=\s*([\S']+))"); // 匹配 "属性1 = 属性2"
        string::const_iterator searchStart(whereClause.cbegin());
        smatch conditionMatch;

        while (regex_search(searchStart, whereClause.cend(), conditionMatch, conditionRegex)) {
            string left = conditionMatch[1];  // 等号左侧的属性
            string right = conditionMatch[2]; // 等号右侧的属性或值

            // 去掉引号（如果右侧是字符串值）
            if (right.front() == '\'' && right.back() == '\'') {
                right = right.substr(1, right.size() - 2);
            }

            conditions.emplace_back(left, right); // 存储为 pair
            searchStart = conditionMatch.suffix().first; // 继续匹配剩余部分
        }
    }

    return conditions;
}

//字符串分割函数split，用于分割.的左右部分
vector<string> split(string& input) {
    vector<string> tokens; // 用于存储分割结果
    regex re(R"(\.)");     // 匹配点 "." 作为分隔符
    sregex_token_iterator it(input.begin(), input.end(), re, -1); // 正则分割
    sregex_token_iterator end;

    while (it != end) {
        tokens.push_back(*it++); // 将分割的每部分存入 tokens
    }

    return tokens;
}


//vector<SqlPlan> parseSql(string sql,map<string,Tableinfo> tableinfo)

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

vector<SqlPlan> parseSql(string sql,map<string,TableInfo> tables)
{
    stringstream ss;
    if (sql.back() == ';') {
        sql.pop_back();
    }
    //测试语句
    // string s = "SELECT id FROM student WHERE name = 'Bob'";

    vector<SqlPlan> plans;
    vector<string> res_param;
    //分别是sql 查询中的表名、最终的投影属性名、where条件
    vector<string> tableNames = parseTables(sql);
    vector<string> projectionNames = parseSelectAttributes(sql);
    vector<pair<string,string>> conditions = parseWhereConditions(sql);
    bool is_join = false;
    bool is_aggregation_sum = isAggregationType(sql);
    bool is_simple_select = false;
    
    string projectionIndex;
    for(int i=0;i<projectionNames.size();i++)
    {
        string projectionName = projectionNames[i];
        vector<string> params= split(projectionName);
        if(params.size()<=1)
            continue;
        TableInfo cur_info = tables[params[0]];
        vector<string> columns = cur_info.get_columns();
        for(int i=0;i<columns.size();i++)
        {
            if(params[1] == columns[i])
            {
                params[1] = to_string(i);
                projectionIndex = to_string(i);
                break;
            }
        }
        projectionNames[i] = params[0] + "," + params[1];
        //cout << projectionName << endl;
    }
    for(int i=0;i<conditions.size();i++) {
        // 处理条件左侧
        pair<string,string> condition = conditions[i];
        vector<string> leftParams = split(condition.first);
        vector<string> rightParams = split(condition.second);
        bool is_cur_join_type = false;
        if(leftParams.size() == rightParams.size()&&leftParams.size()==2)is_cur_join_type  =true;
        if (leftParams.size() > 1) {
            // 如果可以 split，则获取表信息
            TableInfo cur_info = tables[leftParams[0]];
            vector<string> columns = cur_info.get_columns();
            for (int i = 0; i < columns.size(); i++) {
                if (leftParams[1] == columns[i]) {
                    leftParams[1] = to_string(i);
                    break;
                }
            }
            if(is_cur_join_type ) {condition.first = leftParams[0] + "_" + leftParams[1];}
            else condition.first = leftParams[0] + "," + leftParams[1];
        }
        // 处理条件右侧
        if (rightParams.size() > 1) {
            // 如果可以 split，则获取表信息
            TableInfo cur_info = tables[rightParams[0]];
            vector<string> columns = cur_info.get_columns();
            for (int i = 0; i < columns.size(); i++) {
                if (rightParams[1] == columns[i]) {
                    rightParams[1] = to_string(i);
                    break;
                }
            }
            if(is_cur_join_type)condition.second = rightParams[0] + "_" + rightParams[1];
            else condition.second = rightParams[0] + "," + rightParams[1];
            if(is_cur_join_type)is_join = true;
        }
        conditions[i] = condition;
    }

    //判断查询类型



    if(is_join) //如果是连接查询
    {
        
        vector<string> no_quote_list = extractSingleQuoteContent(conditions[1].second);
        string no_quote = conditions[1].second;
        if(!no_quote_list.empty()) {
            no_quote = no_quote_list[0];
        }
        string pp1 = conditions[1].first + "," + no_quote;
        string enc_pp1 = prfFunctionReturnString(pp1,true);
        vector<string> p1 = {enc_pp1};
        SqlPlan pl1("sigma",p1);
        plans.push_back(pl1);

        vector<string> p2 = {conditions[0].first, conditions[0].second};
        SqlPlan pl2("join",p2);
        plans.push_back(pl2);

        vector<string> p3 = {projectionIndex};
        SqlPlan pl3("projection",p3);
        plans.push_back(pl3);

        vector<string> p4 = {"string"};
        SqlPlan pl4("result",p4);
        plans.push_back(pl4);

        return plans;



    }

    else if(is_aggregation_sum)     //如果是包含聚合函数的查询
    {
        vector<string> col_name = tables[tableNames[0]].get_columns();
        vector<string> col_types = tables[tableNames[0]].get_columns_type();
        //cout << parseSumAttributeColIndex(sql,col_name) << endl;
        int colIndex = parseSumAttributeColIndex(sql, col_name);

        res_param.push_back(col_types[colIndex]);
        ss << tables[tableNames[0]].get_name()<< "," << colIndex;

        string p3 = ss.str();
        vector<string> actual_p3 = {prfFunctionReturnString(p3,true)};
        ss.str("");
        SqlPlan pl3("sum",actual_p3);
        plans.push_back(pl3);
    }
    else        //如果是简单查询
    {
        vector<string> col_name = tables[tableNames[0]].get_columns();
        vector<string> col_types = tables[tableNames[0]].get_columns_type();
        int colIndex1 = findColIndex(col_name,conditions[0].first);
        //cout << "所在的列是" << parseWhereConditionColIndex(sql,col_name) << endl;           //where子句后的内容所在的列
        ss << tables[tableNames[0]].get_name() <<","
           << colIndex1 << ","
           << conditions[0].second;
        string p1 = ss.str();
        vector<string> actual_p1 = {prfFunctionReturnString(p1, true)};
        ss.str("");
        SqlPlan pl1("select",actual_p1);
        plans.push_back(pl1);

        //cout << "所在的列是" << parseSelectAttributeColIndex(sql,col_name) << endl;
        int colIndex2 = findColIndex(col_name,projectionNames[0]);//select后的内容所在的列
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

