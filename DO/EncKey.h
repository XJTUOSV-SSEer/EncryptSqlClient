//
// Created by 陈志庭 on 24-9-22.
//

#ifndef ENCKEY_H
#define ENCKEY_H
#include<iostream>
using namespace std;
#define OUT_DATA_FILE "data/key.csv"

/**
 * key 持久化结构，待完成
 */
class EncKey {
public:
    pair<int, int> index;
    string key;
    string iv;
    string MMType;
    /**
     * 索引的字符串形式便于持久化
     */
    string index_str;

    EncKey();
    EncKey(pair<int, int> index,string key,string iv,string MMType);

    /**
     * 将数据存入 csv 中
     * @param fileName
     */
    void toCsv(const string& fileName=OUT_DATA_FILE) const;
    /**
     * 从 csv 中获取 Enckey
     */
    //void LoadFromCsv(const string& fileName=OUT_DATA_FILE,int tar_row = 1);

    /**
     * 重载了输出操作符
     */
    friend ostream &operator<<( ostream &output,const EncKey &E ) {
        output << E.index_str << " " << E.key << " " << E.iv << " " << E.MMType;
        return output;
    }
};


#endif //ENCKEY_H
