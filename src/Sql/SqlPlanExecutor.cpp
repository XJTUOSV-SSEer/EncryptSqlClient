//
// Created by 陈志庭 on 24-11-18.
//

#include "SqlPlanExecutor.h"

#include <sstream>
#include <utility>

#include "../EncryptTools/EncryptUtil.h"
#include "../dataObject/SqlPlan.h"
#include "TokenGenerator.h"
SqlPlanExecutor::SqlPlanExecutor(PGconn *conn,std::vector<SqlPlan> plan){
    this->plans = std::move(plan);
    this->conn = conn;
}


void SqlPlanExecutor::execute() {
    stringstream ss;
    vector<string> tmpTables;

    for (int i =0; i < this->plans.size(); i++) {
        SqlPlan plan = plans[i];
        string tmp_table =  "tmpTable_execTmp_" + to_string(i);
        string from_table;
        if(i!=0) {
            from_table = tmpTables.back();
        }
        ss << "DROP TABLE IF EXISTS " + tmp_table + ";";


        string sqlQuery,targetTable;
        std::string pType = plan.getType();
        if(pType == "select") {
            targetTable = "mmv";
            sqlQuery = generateSigmaQuery(targetTable,tmp_table,plan.getParams()[0]);
            tmpTables.push_back(tmp_table);
            ss <<  "CREATE TABLE IF NOT EXISTS " + tmp_table
            + "(val varchar(255));";
        }
        else if(pType == "join") {
            stringstream sstmp;
            sstmp << "mmjoin_" << plan.getParams()[1] << "_join_" << plan.getParams()[0];
            targetTable =sstmp.str();
            sqlQuery = generateSigmaQueryFromTmpTable(from_table,tmp_table,targetTable);
            ss <<  "CREATE TABLE IF NOT EXISTS " + tmp_table
        + "(val varchar(255));";
        }
        else if(pType == "projection") {
            targetTable = "mmr";
            sqlQuery = generateProjetcionQueryFromTmpTable(from_table,plan.getParams()[0],tmp_table);
            ss <<  "CREATE TABLE IF NOT EXISTS " + tmp_table
        + "(val bytea);";
        }
        else if(pType == "sum") {
            targetTable = "mmc";
            sqlQuery = generateSumQuery(plan.getParams()[0],tmp_table);
            ss <<  "CREATE TABLE IF NOT EXISTS " + tmp_table
        + "(val bytea);";
        }
        else if(pType == "result") {
            break;
        }


        tmpTables.push_back(tmp_table);
        ss << sqlQuery;
        executeQuery(ss.str());
        ss.str("");
    }
    string tmpTable = tmpTables.back();
    SqlPlan result_plan = plans.back();
    string resultSqlQuery = "select * from " + tmpTable;
    executeByteaResultQuery(resultSqlQuery);
    //if(result_plan.getParams()[0] == "int") {
    //    executeByteaResultQuery(resultSqlQuery);
    //}
    //else {
    //    executeResultQuery(resultSqlQuery);
    //}

    deleteTmp(tmpTables);

}

void SqlPlanExecutor::executeQuery(std::string sqlQuery,bool get_res) {
    const char* query = sqlQuery.c_str();
    PQexec(conn, query);
}
void SqlPlanExecutor::executeResultQuery(std::string sqlQuery) {
    const char* query = sqlQuery.c_str();
    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "执行查询失败: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        PQfinish(conn);
    }
    if (res == nullptr) {
        throw std::runtime_error("Invalid PGresult pointer");
    }

    int numRows = PQntuples(res);  // 获取行数
    int numCols = PQnfields(res); // 获取列数

    for (int i = 0; i < numRows; ++i) {
        std::vector<std::string> row;
        for (int j = 0; j < numCols; ++j) {
            char* value = PQgetvalue(res, i, j); // 获取指定单元格的值
            row.push_back(value ? value : "NULL"); // 处理 NULL 值
        }
        results.push_back(row);
    }
}

void SqlPlanExecutor::executeByteaResultQuery(std::string sqlQuery) {
    const char* query = sqlQuery.c_str();
    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "执行查询失败: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        PQfinish(conn);
    }
    if (res == nullptr) {
        throw std::runtime_error("Invalid PGresult pointer");
    }

    int numRows = PQntuples(res);  // 获取行数
    int numCols = PQnfields(res); // 获取列数

    for (int i = 0; i < numRows; ++i) {
        std::vector<std::string> row;
        for (int j = 0; j < numCols; ++j) {
            string::size_type len;
            unsigned char *data = PQunescapeBytea((unsigned char*)PQgetvalue(res, i, j), (size_t*)&len);// 获取指定单元格的值
            string value = {reinterpret_cast<const char*>(data), len};
            row.push_back(data ? value : "NULL"); // 处理 NULL 值
        }
        results.push_back(row);
    }
}
void SqlPlanExecutor::deleteTmp(std::vector<std::string> tmps) {
    stringstream ss;
    for (const auto & tmp : tmps) {
        ss << "DROP TABLE IF EXISTS " << tmp << ";";
        executeQuery(ss.str());
        ss.str("");
    }
}

std::vector<std::vector<std::string>> SqlPlanExecutor::getResults() {
    for(int i=0;i<results.size();i++){
        for(int j=0;j<results[0].size();j++){
            string data = results[i][j];
            string decrypt_data = decryptSymmetricEncryption(data);
            results[i][j] = decrypt_data;
        }
    }

    return results;
}

