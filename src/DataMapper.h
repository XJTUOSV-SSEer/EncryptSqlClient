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

#include "DO/EncryptedMultiMap.h"
#include "DO/RowMultiMap.h"
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
     * @param tableID 表的编号
     * @param inData 数据
     * @param columnsTypes 每列的属性
     * @return
     */
    RowMultiMap rowMultiMapConstruct(int tableID, vector<vector<string>> inData, vector<string> columnsTypes);
    RowMultiMap colMultiMapConstruct(int tableID, vector<vector<string>>inData, vector<string>columnsTypes);

    RowMultiMap valueMultiMapConstruct(int tableID, vector<vector<string>> inData, vector<string> columnsTypes);
    RowMultiMap joinMultiMapConstruct(int tableID1, int tableID2, vector<vector<string>> table1,
                                      vector<vector<string>> table2, int joinCol1, int joinCol2);
    void generateEmmIntoSql(PGconn *conn, int tableID, vector<vector<string>> Table, vector<string> columnsTypes);
    void generateJoinEmmIntoSql(PGconn *conn, int tableID1, int tableID2, vector<vector<string>>table1, vector<vector<string>>table2, int targetCol1, int targetCol2);
    int decryptData(string cipertext);
    static vector<vector<string>> rowMapperDecrypt(RowMultiMap rmm);

    /**
     * 计划从 csv 数据中读入原始数据
     * @param fileName
     * @return
     */
    static vector<vector<string>> fileReader( const string& fileName);
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
         * @param row_text_len 待加入的行/列数据密文长度向量，主要用于对称加密的解密。
         * @param text 文本数据
         */
        void insertIntoRowBySymmetricEncryption(vector<string> &row, vector<int> &row_text_len, const string& text);
        /**
         * 对数据进行同态加密，并加入到 MM 中。key 需要使用 seal::KeyGenerator 生成，并先传入到 rowMultiMapConstruct 中
         * @param row 待加入的行/列向量
         * @param row_text_len 待加入的行/列数据密文长度向量，主要用于对称加密的解密。
         * @param value 文本数据
         */
        void insertIntoRowByHomomorphicEncryption(vector<string> &row, vector<int>&row_text_len,int value);
    void insertEMM(EncryptedMultiMap emm, PGconn *conn,string targetTable,bool value_is_bytea);
    void insertIntoSql(const pair<string, string>&kv,string targetTable,PGconn *conn,bool value_is_bytea);
};



#endif //DATAMAPPER_H
