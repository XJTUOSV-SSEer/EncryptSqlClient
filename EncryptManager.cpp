//
// Created by 陈志庭 on 24-9-25.
//

#include "EncryptManager.h"

#include "main.h"
#include "EncryptTools/Crypto_Primitives.h"
#include "EncryptTools/EncryptUtil.h"

EncryptManager::EncryptManager() {
    this->key_1 = DATA_KEY_1;
    this->key_2 = DATA_KEY_2;
}

EncryptManager:: EncryptManager(string key_1, string key_2){
    this->key_1 = key_1;
    this->key_2 = key_2;
}

EncryptedMultiMap EncryptManager::setup(RowMultiMap mm) {
    /*
     *
     */
    EncryptedMultiMap encr_multi_map;


    set<string> keys = mm.getKeys();
    /*
     * 先遍历 MM 的 key，也就是数据库行的索引信息，得到每个行向量
     */
    for (auto ori_key : keys) {
        vector<string> data = mm.get(ori_key);
        /*
        * 逐个取出行向量中的元素，对索引进行补充，生成随机数后与加密数据一并放入 EMM 中
        */
        for(int i = 0; i < data.size(); i++) {
            string full_index = ori_key + ',' + to_string(i);

            encr_multi_map.insert(prfFunction(full_index),mm.get(ori_key)[i]);

            
        }

    }
    return encr_multi_map;
}

vector<string> EncryptManager::token(const vector<string>& indexes) {
    vector<string> tokens;
    tokens.reserve(indexes.size());
    for(const auto & index : indexes) {
        tokens.push_back(prfFunction(index));
    }
    return tokens;
}

string EncryptManager::token(string index) {
    return prfFunction(std::move(index));
}

string EncryptManager::prfFunction(string full_index) {
    string str_key = this->key_1;
    unsigned char* key = StringToUchar(str_key);
    int key_size = full_index.size() * 8;
    auto* encrypted_key = new unsigned char[full_index.size()];

    Crypto_Primitives::get_prf(key,StringToUchar(full_index),key_size,encrypted_key);

    return UcharToString(encrypted_key);
}
