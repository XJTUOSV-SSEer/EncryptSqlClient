//
// Created by 陈志庭 on 24-11-25.
//

#ifndef ENCRYPTSERVE_H
#define ENCRYPTSERVE_H
#include "DataMapper.h"
#include "dataObject/SqlPlan.h"
#include "paser/paser.h"
#include <iostream>


class SqlPlan;class EncryptService {

    DataMapper dataMapper;
    vector<SqlPlan> currentPlan;
    PGconn *conn;
    Table currentTable;
    map<string,TableInfo> tableMap;
public:

    explicit EncryptService(EncryptionParameters &params);
    /**
     * 设置连接
     * @param conn 对应 PG 连接
     */
    void setConn(PGconn *conn);
    /**
     * 将对应名字的文件读入，加密并传入 Sql 中
     * @param fileName 对应的文件，暂时只支持.csv文件
     */
    void uploadFileIntoSql(const string &fileName);
    /**
     * 将 Table 数据结构加密并传入 Sql 中
     * @param table 需要加密的表信息
     */
    void uploadTableIntoSql(Table table);
    void uploadJoinTableIntoSql(Table table1, Table table2,int join_col1,int join_col2);
    /**
     * 将得到的结果解密并返回
     * @param res 得到的结果
     * @return 解密结果
     */
    vector<vector<string>> decryptedResult(vector<vector<string>> res);
    /**
     * 将二维数组加密并传入 sql
     * @param tableId  表名
     * @param tables 二维数组
     * @param column_types 行属性
     */
    void insertEncryptedTableToSql(int tableId, vector<vector<string>> tables, vector<string> column_types);
    /**
     * 执行 sql 语句
     * @param sql
     * @return sql 执行结果
     */
    vector<vector<string>>  executeSql(string sql);

};



#endif //ENCRYPTSERVE_H
