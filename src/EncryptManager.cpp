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

EncryptedMultiMap EncryptManager::setup(RowMultiMap mm,map<string,string>&idx2key,bool value_to_hex) {
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
            unsigned char* enc_idx = new unsigned char[enc_key_size];
            int key_len = prfFunctionReturnUnsignedChar(full_index, enc_idx);
            string key = unsignedCharArrayToHexString(enc_idx, key_len);
            // 将 prf 函数生成的结果转为 16进制字符 再级联 对应的坐标，再进行对称加密，也就是 H(F(index) || row_index)
            string key_with_index = key + ',' + to_string(i);
            string enc_kwi = getSymmetricEncryption(key_with_index);

            unsigned char kwi_char[enc_kwi.size()];
            StringToUchar(enc_kwi,kwi_char);

            string enc_kwi_hex = unsignedCharArrayToHexString(kwi_char, enc_kwi.size());

            // 做一个索引到 key 的映射，方便查找
            idx2key.insert(pair(full_index,key));

            encr_multi_map.insert(enc_kwi_hex,enc_kwi_hex);


            
        }

    }
    return encr_multi_map;
}
EncryptedMultiMap EncryptManager::setup(RowMultiMap mm,bool value_to_hex) {
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
            const string& full_index = ori_key;
            int enc_key_size = full_index.size();
            if(enc_key_size %16!=0) {
                enc_key_size = enc_key_size / 16 + 16;
            }
            // 利用 prf 生成 F(index)
            unsigned char* enc_idx = new unsigned char[enc_key_size];
            int key_len = prfFunctionReturnUnsignedChar(full_index, enc_idx);
            string key = unsignedCharArrayToHexString(enc_idx, key_len);
            // 将 prf 函数生成的结果转为 16进制字符 再级联 对应的坐标，再进行对称加密，也就是 H(F(index) || row_index)
            string key_with_index = key + ',' + to_string(i);
            string enc_kwi = getSymmetricEncryption(key_with_index);

            unsigned char kwi_char[enc_kwi.size()];
            StringToUchar(enc_kwi,kwi_char);

            string enc_kwi_hex = unsignedCharArrayToHexString(kwi_char, enc_kwi.size());

            // 做一个索引到 key 的映射，方便查找
            //idx2key.insert(pair(full_index,key));

            if(value_to_hex) {
                string mmdata = mm.get(ori_key)[i];
                string mmdata_hex = binaryToHex(mmdata);
                encr_multi_map.insert(enc_kwi_hex,mmdata_hex);
            }
            else {
                encr_multi_map.insert(enc_kwi_hex,mm.get(ori_key)[i]);
            }


        }

    }
    return encr_multi_map;
}
EncryptedMultiMap EncryptManager::setupPerRow(RowMultiMap mm,bool value_to_hex) {
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
            unsigned char* enc_idx = new unsigned char[enc_key_size];
            int key_len = prfFunctionReturnUnsignedChar(full_index, enc_idx);
            string key = unsignedCharArrayToHexString(enc_idx, key_len);
            // 将 prf 函数生成的结果转为 16进制字符 再级联 对应的坐标，再进行对称加密，也就是 H(F(index) || row_index)
            string key_with_index = key + ',' + to_string(i);
            string enc_kwi = getSymmetricEncryption(key_with_index);
            //cout << enc_kwi.size() << endl;
            unsigned char kwi_char[enc_kwi.size()];
            StringToUchar(enc_kwi,kwi_char);

            string enc_kwi_hex = unsignedCharArrayToHexString(kwi_char, enc_kwi.size());
            encr_multi_map.insert(enc_kwi_hex,mm.get(ori_key)[i]);



        }

    }
    return encr_multi_map;
}

vector<string> EncryptManager::token(const vector<string>& indexes) {
    vector<string> tokens;
    tokens.reserve(indexes.size());
    for(const auto & index : indexes) {
        tokens.push_back(prfFunctionReturnString(index));
    }
    return tokens;
}

string EncryptManager::token(string index) {
    return prfFunctionReturnString(std::move(index));
}



string prfFunctionReturnString(const string& full_index,bool return_hex) {
    string str_key = DATA_KEY_1;
    int key_size = full_index.size();
    int enc_key_size = full_index.size();
    if(key_size %16!=0) {
        enc_key_size = key_size / 16 + 16;
    }


    auto* encrypted_key = new unsigned char[enc_key_size];
    auto* full_index_unsigned_char = new unsigned char[full_index.size()];
    auto *key = new unsigned char[str_key.size()];

    StringToUchar(full_index, full_index_unsigned_char);
    StringToUchar(str_key,key);

    string::size_type key_len = Crypto_Primitives::get_prf(key,full_index_unsigned_char,key_size,encrypted_key);

    delete[] full_index_unsigned_char;
    delete[] key;
    if(return_hex) {
        return unsignedCharArrayToHexString(encrypted_key,key_len);
    }

    return {reinterpret_cast<const char*>(encrypted_key), key_len};
}

int prfFunctionReturnUnsignedChar(const string& full_index,unsigned char* encrypted_key) {
    string str_key = DATA_KEY_1;
    int key_size = full_index.size();
    auto* full_index_unsigned_char = new unsigned char[full_index.size()];
    auto *key = new unsigned char[str_key.size()];

    StringToUchar(full_index, full_index_unsigned_char);
    StringToUchar(str_key,key);

    string::size_type key_len = Crypto_Primitives::get_prf(key,full_index_unsigned_char,key_size,encrypted_key);

    delete[] full_index_unsigned_char;
    delete[] key;
    return key_len;
}

