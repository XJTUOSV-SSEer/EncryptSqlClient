//
// Created by 陈志庭 on 24-9-21.
//

#ifndef DATAMAPPER_H
#define DATAMAPPER_H
#include <vector>

#include <fstream>
#include <iostream>
#include <map>
#include <seal/decryptor.h>
#include <seal/encryptor.h>
#include <seal/keygenerator.h>
#include <sstream>
#include <string>
#include <vector>

#include "DO/RowMultiMap.h"
using namespace std;
using namespace seal;

/**
 * 用于判断并生成对应 MM 数据结构的工具类，待完成
 */
class DataMapper {
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
    RowMultiMap rowMultiMapConstruct(int tableID, vector<vector<string>> inData,vector<string> columnsTypes);
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
    private:
        string dataFilePath;
        SEALContext context;
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
         * @param row 待加入的行向量
         * @param row_text_len 待加入的行向量长度
         * @param text 文本数据
         */
        void insertIntoRowBySymmetricEncryption(vector<string> &row, vector<int> &row_text_len, const string& text);
        /**
         * 对数据进行同态加密，并加入到 MM 中。key 需要使用 seal::KeyGenerator 生成，并先传入到 rowMultiMapConstruct 中
         * @param row 待加入的行向量
         * @param row_text_len 待加入的行向量长度
         * @param value 文本数据
         */
        void insertIntoRowByHomomorphicEncryption(vector<string> &row, vector<int>&row_text_len,int value);
};



#endif //DATAMAPPER_H
