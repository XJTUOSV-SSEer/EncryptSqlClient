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
        string tmp_table =  "tmpTable_execTmp_" + to_string(i);
        string from_table;
        if(i!=0) {
            from_table = tmpTables.back();
        }
        ss << "DROP TABLE IF EXISTS " + tmp_table + ";";
        ss <<  "CREATE TABLE IF NOT EXISTS " + tmp_table
            + "(val varchar(255));";

        string sqlQuery,targetTable;
        std::string pType = plan.getType();
        if(pType == "select") {
            targetTable = "mmv";
        }
        else if(pType == "join") {
            stringstream sstmp;
            sstmp << "mmjoin_" << plan.getParams()[0] << "_join_" << plan.getParams()[1];
            targetTable =sstmp.str();
        }
        else if(pType == "projection") {
            targetTable = "mmr";
        }
        //首次查询使用关键字查询，当前仅支持单关键字。
        if(i==0) {
            sqlQuery = generateSigmaQuery(targetTable,tmp_table,plan.getParams()[0]);
            tmpTables.push_back(tmp_table);
        }
        //之后的查询可以直接从临时表中拉取。
        else {
            if(pType == "join")sqlQuery = generateSigmaQueryFromTmpTable(from_table,tmp_table,targetTable);
            if(pType == "projection")sqlQuery = generateProjetcionQueryFromTmpTable(targetTable,plan.getParams()[0],tmp_table);
        }
        ss << sqlQuery;
        executeQuery(ss.str());
    }
    string tmpTable = tmpTables.back();
    string resultSqlQuery = "select * from " + tmpTable;
    executeResultQuery(resultSqlQuery);
    deleteTmp(tmpTables);
}

void SqlPlanExecutor::executeQuery(std::string sqlQuery,bool get_res) {
    const char* query = sqlQuery.c_str();
    PGresult *res = PQexec(conn, query);

    if (get_res && PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "执行查询失败: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        PQfinish(conn);
    }
    if(!get_res && PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "执行查询失败: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        PQfinish(conn);
    }
}
void SqlPlanExecutor::executeResultQuery(std::string sqlQuery) {
    const char* query = sqlQuery.c_str();
    PGresult *res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "执行查询失败: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        PQfinish(conn);
    }
    pg_result = res;
}
void SqlPlanExecutor::deleteTmp(std::vector<std::string> tmps) {
    stringstream ss;
    for (const auto & tmp : tmps) {
        ss << "DROP TABLE IF EXISTS " << tmp << ";";
    }
}

