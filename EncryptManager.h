//
// Created by 陈志庭 on 24-9-25.
//

#ifndef ENCRYPTMANAGER_H
#define ENCRYPTMANAGER_H
#include "DO/EncryptedMultiMap.h"

/**
 * 实现论文中提到的 Setup 和 Token 函数
 */
class EncryptManager {
    private:
    string key_1;
    string key_2;
    string prfFunction(string full_index);
    public:
    EncryptManager();
    EncryptManager(string key_1, string key_2);
    /**
     * setup 函数，用于生成 EMM，此处密钥暂时用固定的代替。
     * @param mm
     * @return
     */
    EncryptedMultiMap setup(RowMultiMap mm);

    /**
     * 根据索引构建 token，此处是 sigma token。
     * @param indexes 索引向量，注意索引精确到某行的某一元素
     * @return 对应的 token 向量
     */
    vector<string> token(const vector<string>& indexes);
   /**
     * 根据索引构建 token，此处是 sigma token。
     * @param index 索引，注意索引精确到某行的某一元素
     * @return 对应的 token
     */
    string token(string index);
};



#endif //ENCRYPTMANAGER_H
