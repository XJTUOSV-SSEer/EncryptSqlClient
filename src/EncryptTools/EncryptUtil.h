//
// Created by 陈志庭 on 24-9-21.
// 存放各类工具函数
//

#ifndef ENCRYPTUTIL_H
#define ENCRYPTUTIL_H
#include <iosfwd>

#endif //ENCRYPTUTIL_H

#include<iostream>
using namespace std;

/**
 * 随机数生成器，生成 length*8 字节长度的密钥
 * @param length
 * @return
 */
string KeyGeneratorForCP(int length);

/**
 * unsigned char数组与string相互转换的工具函数
 */
string UcharToString(unsigned char* input);
void StringToUchar(string input,unsigned char* output);
int StringToUchar2(string input,unsigned char* output);

int getUcharLength(unsigned char* input);
/**
 * 将字符串填充到 16 字节倍数并返回对应的unsigned char，在加密函数调用时使用。
 * @param plainText
 * @param hexData
 * @return 
 */
void padding16(string plainText,unsigned char* hexData);
int ucharLen(unsigned char* input);
unsigned char* joinByArrow(const unsigned char* c1,int c1_len,const unsigned char* c2,int c2_len);
int hexStringToInt(const std::string& hexStr);