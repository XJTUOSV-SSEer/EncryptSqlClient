//
// Created by 陈志庭 on 24-11-18.
//

#include "SqlPlanExecutor.h"

#include <sstream>
#include <utility>

#include "../DO/SqlPlan.h"
#include "../EncryptTools/EncryptUtil.h"
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
        string tmp_table =  "tmpTable_" + generateRandomString();

        ss <<  "CREATE TABLE IF NOT EXISTS " + tmp_table
            + "(enckey VARCHAR(256) NOT NULL PRIMARY KEY, encValue BYTEA);";

        string sqlQuery,targetTable;
        std::string pType = plan.getType();
        vector<string> whereParams = plan.getWhereParams();
        if(pType == "select") {
            targetTable = "mmv";
        }
        else if(pType == "join") {
            targetTable = "mmjoin";
        }
        else if(pType == "projection") {
            targetTable = "mmprojection";
        }
        //首次查询使用关键字查询，当前仅支持单关键字。
        if(i==0) {
            sqlQuery = generateSigmaQuery(targetTable,tmp_table,whereParams[0]);
            tmpTables.push_back(tmp_table);
        }
        //之后的查询可以直接从临时表中拉取。
        else {
            sqlQuery = generateSigmaQueryFromTmpTable(targetTable,tmp_table);
        }
        ss << sqlQuery;
        executeQuery(sqlQuery);


    }
}

void SqlPlanExecutor::executeQuery(std::string sqlQuery,bool get_res) {
    const char* query = sqlQuery.c_str();
    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "执行查询失败: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        PQfinish(conn);
    }
}
void SqlPlanExecutor::deleteTmp(std::vector<std::string> tmps) {
    stringstream ss;
    for (const auto & tmp : tmps) {
        ss << "DELETE TABLE IF EXISTS " << tmp << ";";
    }
}