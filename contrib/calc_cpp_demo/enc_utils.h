//
// Created by 陈志庭 on 24-10-30.
//

#ifndef ENC_UTILS_H
#define ENC_UTILS_H
#include <iostream>
using namespace std;

extern "C" string getSymmetricEncryption(const string& text);
extern "C" string unsignedCharArrayToHexString(const unsigned char* data, size_t length);
extern "C" void StringToUchar(string input,unsigned char* output);
#endif //ENC_UTILS_H
