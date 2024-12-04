//
// Created by 陈志庭 on 24-9-25.
//

#include "EncryptManager.h"

#include "main.h"
#include "EncryptTools/Crypto_Primitives.h"
#include "EncryptTools/EncryptUtil.h"
#include "string"

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


    vector<string> keys = mm.getKeys();
    /*
     * 先遍历 MM 的 key，也就是数据库行的索引信息，得到每个行向量
     */
    for (auto ori_key : keys) {
        vector<string> data = mm.get(ori_key);
        /*
        * 逐个取出行向量中的元素，对索引进行补充，生成随机数后与加密数据一并放入 EMM 中
        */
        for(int i = 0; i < data.size(); i++) {

            string full_index = ori_key;
            int enc_key_size = full_index.size();
            if(enc_key_size %16!=0) {
                enc_key_size = enc_key_size / 16 + 16;
            }
            // 利用 prf 生成 F(index)
            string key = prfFunctionReturnString(full_index, true);
            // 将 prf 函数生成的结果转为 16进制字符 再级联 对应的坐标，再进行取哈希摘要，也就是 H(F(index) || row_index)
            string key_with_index = key + ',' + to_string(i);
            string enc_kwi_hex = getHashEncryption(key_with_index,true);
            encr_multi_map.insert(enc_kwi_hex,mm.get(ori_key)[i]);

        }

    }
    return encr_multi_map;
}




string prfFunctionReturnString(const string& full_index,bool return_hex) {
    string str_key = DATA_KEY_1;
    int key_size = full_index.size();
    int enc_key_size = full_index.size();
    if(key_size %16!=0) {
        enc_key_size = enc_key_size - key_size / 16 + 16;
    }
    if( full_index=="people,1,aaaomksqh") {
        printf("stop here!\n");
    }

    auto* encrypted_key = new unsigned char[enc_key_size + 2];
    auto* full_index_unsigned_char = new unsigned char[full_index.size()];
    auto *key = new unsigned char[str_key.size()];

    StringToUchar(full_index, full_index_unsigned_char);
    StringToUchar(str_key,key);

    string::size_type key_len = Crypto_Primitives::get_prf(key,full_index_unsigned_char,key_size,encrypted_key);
    string res = unsignedCharArrayToHexString(encrypted_key,key_len);
    delete[] encrypted_key;
    delete[] full_index_unsigned_char;
    delete[] key;


    //return {reinterpret_cast<const char*>(encrypted_key), key_len};
    return res;
}

int prfFunctionReturnUnsignedChar(const string& full_index,unsigned char* encrypted_key) {
    string str_key = DATA_KEY_1;
    int key_size = full_index.size();
    auto* full_index_unsigned_char = new unsigned char[full_index.size()+16];
    auto *key = new unsigned char[str_key.size()];

    StringToUchar(full_index, full_index_unsigned_char);
    StringToUchar(str_key,key);

    string::size_type key_len = Crypto_Primitives::get_prf(key,full_index_unsigned_char,key_size,encrypted_key);

    delete[] full_index_unsigned_char;
    delete[] key;
    return key_len;
}

