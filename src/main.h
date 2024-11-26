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

static const string DATA_KEY_1 = "XJTU_ESC_OqHn3xt0CsD1DOlBjkJsc3Q";
static const string DATA_IV_1 = "XJTU_ESC_10s2YIx";
static const string DATA_KEY_2 = "XJTU_ESC_2fRFsbJQMHVU5gLWEULNtXF";
static const string DATA_IV_2 = "XJTU_ESC_nS0x9ep";
static const string PGSQL_CONNINFO = "dbname=test user=chenzhiting password=123456 hostaddr=127.0.0.1 port=5432";
static const string PGSQL_CONNINFO_remote = "dbname=test user=czt password=123456 hostaddr=10.181.7.139 port=5432";

inline std::string uint64_to_hex_string(std::uint64_t value)
{
    return seal::util::uint_to_hex_string(&value, std::size_t(1));
}
inline std::string intToHexString(int num) {
    std::stringstream ss;
    ss << std::hex << num; // 将整数以16进制格式写入字符串流
    return ss.str();
}