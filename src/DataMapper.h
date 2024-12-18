//
// Created by 陈志庭 on 24-9-21.
//

#ifndef DATAMAPPER_H
#define DATAMAPPER_H
#include <vector>

#include <fstream>
#include <iostream>
#include <libpq-fe.h>
#include <map>
#include <seal/decryptor.h>
#include <seal/encryptor.h>
#include <seal/keygenerator.h>
#include <sstream>
#include <string>
#include <vector>

#include "dataObject/dataObject.h"
using namespace std;
using namespace seal;

/**
 * 用于判断并生成对应 MM 数据结构的工具类，待完成
 */
class DataMapper {
    void insert(const pair<string,string>& kv,PGconn *conn);
public:
    DataMapper(EncryptionParameters&parms);
    PublicKey initializePublicKey();
    /**
     * 构建行向量多映射结构
     * @param table_name
     * @param inData 数据
     * @param columnsTypes 每列的属性
     * @return
     */
    RowMultiMap rowMultiMapConstruct( string table_name, vector<vector<string>> inData, vector<string> columnsTypes);
    RowMultiMap colMultiMapConstruct( string table_name, vector<vector<string>> inData, vector<string> columnsTypes);

    RowMultiMap valueMultiMapConstruct(string table_name, vector<vector<string>> inData, vector<string> columnsTypes);
    //
    RowMultiMap joinMultiMapConstruct(string table_name1, string table_name2, vector<vector<string>> table1, vector<vector<string>> table2, int joinCol1,int joinCol2);
    void generateEmmIntoSql(PGconn *conn, int tableID, vector<vector<string>> Table, vector<string> columnsTypes);
    void generateEmmIntoSql(PGconn *conn, string table_name, vector<vector<string>> table, vector<string> types);
    /**
     *  构建据 table1查找 table2 的 mmjoin 表
     */
    void generateJoinEmmIntoSql(PGconn *conn, string table_name1, string table_name2, vector<vector<string>> table1,
                                vector<vector<string>> table2, int joinCol1, int joinCol2);
    /**
     *  构建据 table1查找 table2 的 mmjoin 表
     */
    void generateJoinEmmIntoSql(PGconn *conn, int tableID1, int tableID2, vector<vector<string>>table1, vector<vector<string>>table2, int joinCol1, int joinCol2);
    int decryptData(string cipertext);
    static vector<vector<string>> rowMapperDecrypt(RowMultiMap rmm);

    /**
     * 计划从 csv 数据中读入原始数据
     * @param fileName
     * @return
     */
    static Table fileReader( const string& fileName,bool is_first_name_and_second_type = false);
    SecretKey getSecretKey();
    PublicKey getPublicKey();
    SEALContext context;
    private:
        string dataFilePath;
    //TODO 一些 Seal 参数，需要抽象出去

        KeyGenerator keygen;
        Encryptor encryptor;
        Decryptor decryptor;
        PublicKey public_key;
        SecretKey secret_key;
        /**
         *  维护一个加密密钥的映射
         */
        map<string, string> keyMap;

        /**
         * 对数据进行对称加密，并加入到 MM 中。key 来源于 main.h 中。
         * @param row 待加入的行/列向量
         * @param text 文本数据
         */
        void insertIntoRowBySymmetricEncryption(vector<string> &row,  const string &text);
        void insertIntoRowByPrfEncryption(vector<string> &row,  const string&text);
        /**
         * 对数据进行同态加密，并加入到 MM 中。key 需要使用 seal::KeyGenerator 生成，并先传入到 rowMultiMapConstruct 中
         * @param row 待加入的行/列向量
         * @param value 文本数据
         */
        void insertIntoRowByHomomorphicEncryption(vector<string> &row, int value);
    void insertEMM(EncryptedMultiMap emm, PGconn *conn,string targetTable,bool value_is_bytea);void createTableIfNotExsit(PGconn*conn, string table_name,bool value_is_bytea);
    void insertIntoSql(const pair<string, string>&kv,string targetTable,PGconn *conn,bool value_is_bytea);
    static void load_keys_from_file(SEALContext&context, PublicKey&public_key, SecretKey&secret_key);
};



#endif //DATAMAPPER_H
