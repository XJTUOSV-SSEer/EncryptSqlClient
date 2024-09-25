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
string KeyGenerator(int length);

/**
 * unsigned char数组与string相互转换的工具函数
 */
string UcharToString(unsigned char* input);
unsigned char* StringToUchar(string& input);
/**
 * 将字符串填充到 16 字节倍数并返回对应的unsigned char，在加密函数调用时使用。
 * @param plainText
 * @return 
 */
unsigned char* padding16(string plainText);
int ucharLen(unsigned char* input);