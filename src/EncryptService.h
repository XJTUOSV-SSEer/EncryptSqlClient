//
// Created by 陈志庭 on 24-11-25.
//

#ifndef ENCRYPTSERVE_H
#define ENCRYPTSERVE_H
#include "DataMapper.h"
#include "dataObject/SqlPlan.h"
#include "paser/paser.h"


class SqlPlan;class EncryptService {
private:
    DataMapper dataMapper;
    vector<SqlPlan> currentPlan;
    PGconn *conn;

public:

    explicit EncryptService(EncryptionParameters &params);
    void setConn(PGconn *conn);
    void updateFileIntoSql(const string &fileName);
    void insertEncryptedTableToSql(int tableId, vector<vector<string>> tables, vector<string> column_types);
    vector<vector<string>>  excuteSql(string sql);
};



#endif //ENCRYPTSERVE_H
