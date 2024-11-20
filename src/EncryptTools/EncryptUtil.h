//
// Created by 陈志庭 on 24-9-21.
// 存放各类工具函数
//

#ifndef ENCRYPTUTIL_H
#define ENCRYPTUTIL_H
#include <iosfwd>
#include <seal/secretkey.h>

#endif //ENCRYPTUTIL_H

#include<iostream>
#include <vector>
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
string UcharToString(unsigned char* input,int uchar_len);
void StringToUchar(string input,unsigned char* output);
int StringToUchar2(string input,unsigned char* output);

int getUcharLength(unsigned char* input);
/**
 * 将字符串填充到 16 字节倍数并返回对应的unsigned char，在加密函数调用时使用。
 * @param plainText
 * @param hexData
 * @return 
 */
void padding16(string plainText, unsigned char *hexData);
/**
 * 将十六进制字符串形式的的数字转换为 int 形式
 * @param hexStr
 * @return
 */
int hexStringToInt(const std::string& hexStr);
/**
 * 将unsigned char 数组转换成十六进制字符串以便查询
 * @param data 待转换的unsigned char数组
 * @param length unsigned char数组的长度
 * @return
 */
string unsignedCharArrayToHexString(const unsigned char *data, size_t length);
/**
 * 将十六进制字符串转换为 unsigned char 数组
 * @param hexString
 * @return
 */
vector<unsigned char> hexStringToUnsignedCharArray(const std::string& hexString);
/**
 * 对字符串进行对称加密
 * @param text 待加密字符串
 * @return 加密后字符串长度
 */
string getSymmetricEncryption(const string &text, bool return_hex = false);

string encodeHexString(const std::string &str);
std::string generateRandomString(int length = 32);
void stringToChar(string str,char* output);
std::string binaryToHex(const std::string& binaryStr);

void create_save_keys_to_file();
