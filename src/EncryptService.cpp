//
// Created by 陈志庭 on 24-11-25.
//

#include "EncryptService.h"


#include <utility>

#include "EncryptTools/EncryptUtil.h"
#include "Sql/SqlPlanExecutor.h"
EncryptService::EncryptService(EncryptionParameters &params) : dataMapper(params) {

}
void EncryptService::insertEncryptedTableToSql(int tableId,vector<vector<string>> tables,vector<string> column_types) {
    dataMapper.generateEmmIntoSql(conn,tableId,std::move(tables),std::move(column_types));
}
void EncryptService::setConn(PGconn *conn) {
    this->conn = conn;
}
vector<vector<string>> EncryptService::executeSql(string sql){
    // TODO 1
    vector<SqlPlan> plans = parseSql(std::move(sql),tableMap);
    this->currentPlan = plans;
    SqlPlanExecutor sql_plan_executor(conn,plans);
    sql_plan_executor.execute();
    // TODO 2 完成错误类
    // Result res;

    vector<vector<string>> res = decryptedResult(sql_plan_executor.getResults());


    return res;
}

void EncryptService::updateFileIntoSql(const string &fileName) {
    Table table = DataMapper::fileReader(fileName,true);
    int name_final_idx = fileName.find_first_of(".");
    string table_name = fileName.substr(0,name_final_idx);
    table.set_name(table_name);
    this->currentTable = table;
    this->tableMap[table_name] = table;
    dataMapper.generateEmmIntoSql(conn,table_name,table.get_table(),table.get_columns_type());
}
void EncryptService::uploadTableIntoSql(Table table){
    this->currentTable = table;
    dataMapper.generateEmmIntoSql(conn,table.get_name(),table.get_table(),table.get_columns_type());
}


vector<vector<string>> EncryptService::decryptedResult(vector<vector<string>> res) {
    SqlPlan resPLan = currentPlan.back();
    string resultType = resPLan.getParams()[0];
    for(int i=0;i<res.size();i++) {
        for(int j=0;j<res[0].size();j++) {
            string decryptedData;
            if(resultType == "string") {
                string data = res[i][j];
                decryptedData = decryptSymmetricEncryption(data);
            }
            else if(resultType == "int") {
                string data = res[i][j];
                decryptedData = to_string(dataMapper.decryptData(data));
            }
            else {
                cerr << "Unsupported result type" << endl;
            }
            res[i][j] = decryptedData;
        }
    }
    return res;
}