//
// Created by 陈志庭 on 24-11-18.
//

#ifndef SQLPLANEXECUTOR_H
#define SQLPLANEXECUTOR_H
#include <libpq-fe.h>
#include <vector>


class SqlPlan;class SqlPlanExecutor {
private:
    std::vector<SqlPlan> plans;
    PGconn* conn;
    void executeQuery(std::string sql,bool get_res=false);
    void deleteTmp(std::vector<std::string> tmps);
public:
    SqlPlanExecutor();
    SqlPlanExecutor(PGconn* conn, std::vector<SqlPlan> plans);
    void setConn(PGconn* conn);
    void setPlans(std::vector<SqlPlan> plans);
    void execute();

};



#endif //SQLPLANEXECUTOR_H
