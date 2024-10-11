//
// Created by 陈志庭 on 24-9-21.
//

#ifndef DATAMAPPER_H
#define DATAMAPPER_H
#include <vector>

#include <fstream>
#include <iostream>
#include <map>
#include <seal/encryptor.h>
#include <sstream>
#include <string>
#include <vector>

#include "DO/RowMultiMap.h"
using namespace std;
using namespace seal;

/**
 * 用于判断并生成对应 MM 数据结构的类，待完成
 */
class DataMapper {
    public:
    DataMapper();
    /**
     * 构建行向量多映射结构
     * @param tableID 表的编号
     * @param inData 数据
     * @param columnsTypes 每列的属性
     * @return
     */
    static RowMultiMap rowMapperConstruct(int tableID,vector<vector<string>> inData,vector<string> columnsTypes,const PublicKey& public_key);
    static vector<vector<string>> rowMapperDecrypt(RowMultiMap rmm);

    /**
     *
     * @param fileName
     * @return
     */
    static vector<vector<string>> fileReader( const string& fileName);
    private:
        string dataFilePath;
    /**
     *  维护一个加密密钥的映射
         */
        map<string, string> keyMap;
        static void insertIntoRowBySymmetricEncryption(vector<string> &row, vector<int> &row_text_len, string text);
        static void insertIntoRowByHomomorphicEncryption(vector<string> &row, vector<int>&row_text_len,int value,const PublicKey&public_key);
};



#endif //DATAMAPPER_H
