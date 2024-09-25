#include <iostream>
#include "DataMapper.h"
#include "DO/EncKey.h"
#include "EncryptTools/EncryptUtil.h"
#include "EncryptTools/Crypto_Primitives.h"
#include "main.h"
using namespace std;


/**
 * 测试 AES 加解密
 */
void test() {
    string text = "Hello World!";
    auto* plaintext = padding16(text);
    int textlen  = strlen(reinterpret_cast<char*>(plaintext));
    auto* ciphertext = new unsigned char[textlen];
    auto* plaintext2 = new unsigned char[textlen];
    pair index(0,0);
    string MMType = "row";
    string key =KeyGenerator(32);
    string iv = KeyGenerator(16);
    EncKey ek = EncKey(index,key,iv,MMType);

    unsigned char ciphertext2[textlen];

    int cipertext_len = Crypto_Primitives::sym_encrypt(plaintext,textlen*8,StringToUchar(key),StringToUchar(iv),ciphertext);
    memcpy(ciphertext2,ciphertext,cipertext_len);
    Crypto_Primitives::sym_decrypt(ciphertext,cipertext_len,StringToUchar(key),StringToUchar(iv),plaintext2);

    cout << ciphertext2 << endl;
    cout << plaintext2 << endl;

}

/**
 * 测试 prf 随机生成函数。
 */
void testSetup() {
    string str_key = DATA_KEY_2;
    unsigned char* key = StringToUchar(str_key);
    string ori_key = "0,1";
    string full_index = ori_key + ',' + to_string(0);
    int key_size = full_index.size() * 8;
    auto* encrypted_key = new unsigned char[full_index.size()];
    Crypto_Primitives::get_prf(key,StringToUchar(full_index),key_size,encrypted_key);

    printf("enc_key = %s\n",reinterpret_cast<char*>(encrypted_key));
}

int main() {
    testSetup();
    return 0;
}
