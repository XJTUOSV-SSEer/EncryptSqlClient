//
// Created by 陈志庭 on 24-11-25.
//

#include "EncryptService.h"


#include <utility>

#include "Sql/SqlPlanExecutor.h"
EncryptService::EncryptService(EncryptionParameters &params) : dataMapper(params) {

}
void EncryptService::insertEncryptedTableToSql(int tableId,vector<vector<string>> tables,vector<string> column_types) {
    dataMapper.generateEmmIntoSql(conn,tableId,std::move(tables),std::move(column_types));
}
void EncryptService::setConn(PGconn *conn) {
    this->conn = conn;
}
vector<vector<string>> EncryptService::excuteSql(string sql){
    // TODO 1
    vector<SqlPlan> plans = parseSql(sql,this->currentTable);
    SqlPlanExecutor sql_plan_executor(conn,plans);
    sql_plan_executor.execute();
    // TODO 2 完成错误类
    // Result res;
    vector<vector<string>> res = sql_plan_executor.getResults();

    return res;
}

void EncryptService::updateFileIntoSql(const string &fileName) {
    Table table = DataMapper::fileReader(fileName,true);
    int name_final_idx = fileName.find_first_of(".");
    string table_name = fileName.substr(0,name_final_idx);
    table.set_name(table_name);
    this->currentTable = table;
    dataMapper.generateEmmIntoSql(conn,table_name,table.get_table(),table.get_columns_type());
}