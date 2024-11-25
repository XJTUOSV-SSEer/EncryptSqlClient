//
// Created by Zhiti on 2024/11/16.
//

#include "Token.h"

#include <cstring>
#include <utility>
Token::Token(string sqlQuery, vector<string> paramValues, vector<int> paramLengths, vector<int> paramFormats) {
    this->sqlQuery = std::move(sqlQuery);
    this->paramValues = std::move(paramValues);
    this->paramLengths = std::move(paramLengths);
    this->paramFormats = std::move(paramFormats);
}
const char *Token::getQuery() {
    return sqlQuery.c_str();

}
const char **Token::getparamValues() {
    const char** constCharArray = new const char*[paramValues.size()];

    for (size_t i = 0; i < paramValues.size(); ++i) {
        // 将每个字符串的 C 风格指针赋值到数组
        constCharArray[i] = paramValues[i].c_str();
    }

    return constCharArray;
}
int *Token::getparamLengths() {
    int* intArray = new int[paramLengths.size()];

    // 将 std::vector<int> 的元素复制到 int 数组
    for (size_t i = 0; i < paramLengths.size(); ++i) {
        intArray[i] = paramLengths[i];
    }

    return intArray;
}
int *Token::getparamFormats() {
    int* intArray = new int[paramFormats.size()];

    // 将 std::vector<int> 的元素复制到 int 数组
    for (size_t i = 0; i < paramFormats.size(); ++i) {
        intArray[i] = paramFormats[i];
    }

    return intArray;
}
