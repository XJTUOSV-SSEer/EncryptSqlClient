//
// Created by 陈志庭 on 24-10-30.
//
extern "C" {
#include "postgres.h"
#include "fmgr.h"
#include "Crypto_Primitives.h"


}
#include <iostream>
#include <sstream>
using namespace std;
extern "C"  void StringToUchar(string input,unsigned char* output){
    for(int i=0;i<input.size();i++) {
        output[i] = input[i];
    }
}
extern "C" void padding16(string plainText,unsigned char* hexData) {
    int padLength = plainText.size();
    if(plainText.size() % 16 != 0) {
        padLength = plainText.size()-plainText.size() % 16 +16;
    }

    for(int i=0;i<padLength;i++) {
        if(i<plainText.size()) {
            hexData[i] = plainText[i];
        }
        else {
            hexData[i] = '0';
        }
    }

}
extern "C" string getSymmetricEncryption(const string& text){
    string key = "XJTU_ESC_OqHn3xt0CsD1DOlBjkJsc3Q";
    string iv = "XJTU_ESC_10s2YIx";

    int padLength = text.size();
    if(text.size() % 16 != 0) {
        padLength = text.size()-text.size() % 16 +16;
    }


    auto* plain_text = new unsigned char[padLength];
    auto* ciphertext = new unsigned char[padLength + 16];
    auto* key_uc = new unsigned char[key.size()];
    auto* iv_uc = new unsigned char[iv.size()];

    StringToUchar(key,key_uc);
    StringToUchar(iv,iv_uc);
    //StringToUchar(text,plain_text);

    padding16(text,plain_text);
    int cipertext_len = sym_encrypt(plain_text,padLength,key_uc,iv_uc,ciphertext);
    //string cipherStr = UcharToString(ciphertext,cipertext_len);
    string cipherStr = string(reinterpret_cast<const char*>(ciphertext), cipertext_len);


    delete[] plain_text;
    delete[] key_uc;
    delete[] iv_uc;

    return cipherStr;
}
