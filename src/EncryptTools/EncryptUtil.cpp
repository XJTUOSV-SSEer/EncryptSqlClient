//
// Created by 陈志庭 on 24-9-21.
//
#include "EncryptUtil.h"
#include <random>


string KeyGeneratorForCP(int length) {
    string RAND_STRING_DICTIONARY = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;  // 随机设备
    std::mt19937 generator(rd());  // Mersenne Twister 随机数生成器
    std::uniform_int_distribution<> distribution(0, RAND_STRING_DICTIONARY.size() - 1);

   string key;
    for(int i=0;i<length;i++) {
        key.push_back(RAND_STRING_DICTIONARY[distribution(generator)]);;
    }
    return key;
};

string UcharToString(unsigned char* input)
{
    string output;
    int uchar_len =strlen(reinterpret_cast<char*>(input));
    for(int i=0;i<uchar_len;i++) {
        output.push_back(input[i]);
    }
    return output;
}


unsigned char* StringToUchar(string& input){
    unsigned char *output = new unsigned char[input.size()];
    for(int i=0;i<input.size();i++) {
        output[i] = input[i];
    }
    return output;
}
pair<unsigned char*,int> StringToUchar2(string& input){
    unsigned char *output = new unsigned char[input.size()];
    for(int i=0;i<input.size();i++) {
        output[i] = input[i];
    }
    return pair<unsigned char*,int>(output,input.size());
}

unsigned char* KeyGeneratorBin(int length) {
    unsigned char *key = (unsigned char *) malloc(length);
    for(int i=0;i<length;i++) {
        key[i] = rand() % 256;
    }
    return key;
};

unsigned char* padding16(string plainText) {
    int padLength = plainText.size();
    if(plainText.size() % 2 != 0) {
        padLength = plainText.size()-plainText.size() % 2 +2;
    }

    auto* hexData = new unsigned char[padLength];
    for(int i=0;i<padLength;i++) {
        if(i<plainText.size()) {
            hexData[i] = plainText[i];
        }
        else {
            hexData[i] = '0';
        }
    }
    return hexData;

}

int getUcharLength(unsigned char* input) {
    return strlen(reinterpret_cast<char*>(input));
}

unsigned char* joinByArrow(const unsigned char* c1,int c1_len,const unsigned char* c2,int c2_len) {
    int uCharLength = c1_len + c2_len +2;
    auto* output = new unsigned char[uCharLength];
    for(int i=0;i<(uCharLength);i++) {
        if(i<c1_len) {
            output[i] = c1[i];
        }
        else if(i==c1_len) {
            output[i] = '=';
        }
        else if(i==c1_len+1) {
            output[i] = '>';
        }
        else {
            output[i] = c2[i-c1_len-2];
        }
    }
    return output;
}
int hexStringToInt(const std::string& hexStr) {
    size_t idx = 0;
    int result;

    // 检查字符串是否为空
    if (hexStr.empty()) {
        throw std::invalid_argument("输入的字符串为空");
    }

    try {
        // 使用 std::stoi 进行转换，基数为16
        result = std::stoi(hexStr, &idx, 16);

        // 检查是否有未处理的字符
        if (idx != hexStr.length()) {
            throw std::invalid_argument("包含非16进制字符");
        }
    } catch (const std::invalid_argument&) {
        throw std::invalid_argument("无效的16进制字符串");
    } catch (const std::out_of_range&) {
        throw std::out_of_range("数值超出范围");
    }

    return result;
}