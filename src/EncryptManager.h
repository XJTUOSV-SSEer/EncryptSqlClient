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

    public:
    EncryptManager();
    EncryptManager(string key_1, string key_2);
    /**
     * setup 函数，用于生成 EMM，此处密钥暂时用固定的代替。
     * @param mm
     * @param idx2key
     * @return
     */
    EncryptedMultiMap setup(RowMultiMap mm, map<string, string> &idx2key);
    EncryptedMultiMap setupPerRow(RowMultiMap mm);

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
/**
 * 将索引值进行 prf 加密，计划全部转换成输出 unsigned char 形式以便转换成 16 进制字符串
 * @param full_index
 * @param return_hex 是否将结果转为 16进制字符 表示
 * @return
 */
string prfFunctionReturnString(const string& full_index,bool return_hex = false);
/**
 * 将索引值进行 prf 加密，计划全部转换成输出 unsigned char 形式以便转换成 16 进制字符串
 * @param full_index
 * @return
 */
int prfFunctionReturnUnsignedChar(const string& full_index,unsigned char* encrypted_key);
#endif //ENCRYPTMANAGER_H
