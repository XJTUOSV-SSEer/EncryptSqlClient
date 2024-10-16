//
// Created by 陈志庭 on 24-9-25.
//

#ifndef MAIN_H
#define MAIN_H
#include <seal/util/uintcore.h>

#endif //MAIN_H
#include <string>
#include <sstream>

using namespace std;
inline const string DATA_PATH = "/Users/chenzhiting/ProjectSE/Client_demo/EncryptSqlClient/data/";
inline const string DATA_KEY_1 = "XJTU_ESC_OqHn3xt0CsD1DOlBjkJsc3Q";
inline const string DATA_IV_1 = "XJTU_ESC_10s2YIx";
inline const string DATA_KEY_2 = "XJTU_ESC_2fRFsbJQMHVU5gLWEULNtXF";
inline const string DATA_IV_2 = "XJTU_ESC_nS0x9ep";
inline const string PGSQL_CONNINFO = "dbname=test user=chenzhiting password=123456 hostaddr=127.0.0.1 port=5432";

inline std::string uint64_to_hex_string(std::uint64_t value)
{
    return seal::util::uint_to_hex_string(&value, std::size_t(1));
}
inline std::string intToHexString(int num) {
    std::stringstream ss;
    ss << std::hex << num; // 将整数以16进制格式写入字符串流
    return ss.str();
}