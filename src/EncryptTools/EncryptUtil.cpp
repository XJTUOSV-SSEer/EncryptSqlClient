//
// Created by 陈志庭 on 24-9-21.
//
#include "EncryptUtil.h"

#include <iomanip>
#include <random>
#include <sstream>


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

string UcharToString(unsigned char* input,int uchar_len)
{
    string output;

    for(int i=0;i<uchar_len;i++) {
        output.push_back(input[i]);

    }
    delete[] input;
    return output;
}

//TODO，自动释放 unsigned char 或者 使用其他类型
void StringToUchar(string input,unsigned char* output){
    for(int i=0;i<input.size();i++) {
        output[i] = input[i];
    }
}
int StringToUchar2(string input,unsigned char* output){
    for(int i=0;i<input.size();i++) {
        output[i] = input[i];
    }
    return input.size();
}

unsigned char* KeyGeneratorBin(int length) {
    unsigned char *key = (unsigned char *) malloc(length);
    for(int i=0;i<length;i++) {
        key[i] = rand() % 256;
    }
    return key;
};

void padding16(string plainText,unsigned char* hexData) {
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

std::string unsignedCharArrayToHexString(const unsigned char* data, size_t length) {
    std::ostringstream oss;

    for (size_t i = 0; i < length; ++i) {
        // 将每个字节转换为两位十六进制，并确保填充零
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
    }

    return oss.str(); // 返回十六进制字符串
}

std::vector<unsigned char> hexStringToUnsignedCharArray(const std::string& hexString) {
    std::vector<unsigned char> byteArray;
    size_t length = hexString.length();

    // 每两个字符解析为一个字节
    for (size_t i = 0; i < length; i += 2) {
        // 确保不会越界
        if (i + 1 < length) {
            std::string byteString = hexString.substr(i, 2); // 获取两个字符
            unsigned char byte = static_cast<unsigned char>(std::stoul(byteString, nullptr, 16)); // 转换为 unsigned char
            byteArray.push_back(byte); // 添加到数组中
        }
    }


    return byteArray;
}