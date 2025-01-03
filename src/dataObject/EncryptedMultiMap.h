//
// Created by 陈志庭 on 24-9-25.
//

#ifndef ENCRYPTEDMULTIMAP_H
#define ENCRYPTEDMULTIMAP_H
#include <map>
#include <string>

#include "RowMultiMap.h"
using namespace std;

/**
 * 实现加密多映射结构
 */
class EncryptedMultiMap {
private:
    /**
     * 存放 EMM 结构，此处采用 map 实现。当前 Key 部分采用经过十六进制字符串转换后的字符串进行存储
     */
    map<string,string> KVmap;
    /**
     * 维护一个所有 key 也就是索引信息的集合，用于遍历。
     */
    vector<string> keys;
public:
    EncryptedMultiMap();

    /**
     * 插入一个加密数据对，格式为(F(K),Enc_r)
     * @param key 加密后的 key
     * @param value 加密后的数据
     */
    void insert(string key, string value);

    /**
     * 使用 key 从数据库中得到一个加密数据
     * @param key 加密后的索引
     * @return 加密数据
     */
    string get(string key);
   /**
    * 获得所有索引
    * @return 索引向量
    */
    vector<string> getKeys();
};



#endif //ENCRYPTEDMULTIMAP_H
