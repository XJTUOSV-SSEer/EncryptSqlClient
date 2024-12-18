//
// Created by 陈志庭 on 24-11-18.
//

#ifndef SQLPLANEXECUTOR_H
#define SQLPLANEXECUTOR_H
#include <libpq-fe.h>
#include <vector>
#include <string>

class SqlPlan;
/**
 * 执行对应的 sqlplan，主要是将需要的计划转换成对应的 sql 语句
 */
class SqlPlanExecutor {
private:
    std::vector<SqlPlan> plans;
    PGconn *conn;
    std::vector<std::vector<std::string>> results;
    void executeQuery(std::string sql, bool get_res = false);
    void executeResultQuery(std::string sqlQuery);
    void executeByteaResultQuery(std::string sqlQuery);
    void deleteTmp(std::vector<std::string> tmps);
public:
    SqlPlanExecutor(PGconn* conn, std::vector<SqlPlan> plans);
    void setConn(PGconn* conn);
    void setPlans(std::vector<SqlPlan> plans);
    void execute();
    std::vector<std::vector<std::string>> getResults();
    std::string getResType();

};



#endif //SQLPLANEXECUTOR_H
