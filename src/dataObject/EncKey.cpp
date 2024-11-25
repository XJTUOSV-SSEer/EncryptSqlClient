//
// Created by 陈志庭 on 24-9-22.
//

#include "EncKey.h"
#include "../EncryptTools/EncryptUtil.h"
#include <fstream>
#include <sstream>
#include <utility>

EncKey::EncKey(pair<int, int> index,string key,string iv,string MMType){
    this->index = index;
    this->key = std::move(key);
    this->iv = std::move(iv);
    this->MMType = MMType;

    this->index_str = std::to_string(this->index.first) + "_" + std::to_string(this->index.second);
}
void EncKey::toCsv(const string& fileName) const {
    ofstream outFile;
    outFile.open(fileName,ios::out);
    if(outFile.is_open()) {
        outFile << this->index_str <<this->MMType <<this->key << this->iv << endl;
    }
    outFile.close();
}

//void EncKey::LoadFromCsv(const string& fileName,int tar_row) {
//    std::ifstream file(fileName);  // 打开 CSV 文件
//    if (!file.is_open()) {
//        std::cerr << "Failed to open file." << std::endl;
//        return;
//    }
//    std::string line;
//    std::vector<std::vector<std::string>> data;  // 用于存储 CSV 的所有数据
//
//    int cnt_row = 0;
//    // 逐行读取 CSV 文件
//    while (std::getline(file, line)) {
//        std::vector<std::string> row;
//        std::stringstream ss(line);
//        std::string value;
//
//        // 按逗号分隔每一行的内容
//        while (std::getline(ss, value, ',')) {
//            row.push_back(value);  // 将每一列数据存入当前行
//        }
//        if(cnt_row == tar_row) {
//            this->index_str = row[0];
//            this->MMType = row[1];
//            StringToUchar(row[2],this->key);
//            StringToUchar(row[3],this->iv);
//            break;
//        }
//        data.push_back(row);  // 将整行数据存入二维数据结构
//        cnt_row++;
//    }
//
//    file.close();
//}
//



