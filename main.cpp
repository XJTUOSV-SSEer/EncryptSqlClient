#include "main.h"
#include <iostream>
#include <libpq-fe.h>
#include <string>
#include "DataMapper.h"
#include "EncryptManager.h"
#include "DO/EncKey.h"
#include "DO/EncryptedMultiMap.h"
#include "EncryptTools/Crypto_Primitives.h"
#include "EncryptTools/EncryptUtil.h"
using namespace std;


/**
 * 测试 AES 加解密
 */
void testAES() {
    string text = "Hello World!";
    auto* plaintext = padding16(text);
    int textlen  = strlen(reinterpret_cast<char*>(plaintext));
    auto* ciphertext = new unsigned char[textlen];
    auto* plaintext2 = new unsigned char[textlen];
    pair index(0,0);
    string MMType = "row";
    string key =KeyGenerator(32);
    string iv = KeyGenerator(16);
    EncKey ek = EncKey(index,key,iv,MMType);

    unsigned char ciphertext2[textlen];

    int cipertext_len = Crypto_Primitives::sym_encrypt(plaintext,textlen*8,StringToUchar(key),StringToUchar(iv),ciphertext);
    int test_len = getUcharLength(ciphertext);
    memcpy(ciphertext2,ciphertext,cipertext_len);
    Crypto_Primitives::sym_decrypt(ciphertext,cipertext_len,StringToUchar(key),StringToUchar(iv),plaintext2);

    cout << ciphertext2 << endl;
    cout << plaintext2 << endl;

}

/**
 * 测试 prf 随机生成函数。
 */
void testSetup() {
    string str_key = DATA_KEY_2;
    unsigned char* key = StringToUchar(str_key);
    string ori_key = "0,1";
    string full_index = ori_key + ',' + to_string(0);
    int key_size = full_index.size() * 8;
    auto* encrypted_key = new unsigned char[full_index.size()];
    Crypto_Primitives::get_prf(key,StringToUchar(full_index),key_size,encrypted_key);

    printf("enc_key = %s\n",reinterpret_cast<char*>(encrypted_key));
}

void testQuery() {
    string sql_query = "SELECT * FROM kvtest;";
    string conninfo = PGSQL_CONNINFO;

    // 建立连接
    PGconn *conn = PQconnectdb(conninfo.c_str());
    // 执行SQL查询
    PGresult *res = PQexec(conn, sql_query.c_str());

    // 检查查询结果状态
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Query failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        return;
    }

    // 获取行数和字段数
    int nrows = PQntuples(res);
    int nfields = PQnfields(res);

    std::cout << "Number of rows: " << nrows << std::endl;
    std::cout << "Number of fields: " << nfields << std::endl;

    // 输出结果
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < nfields; j++) {
            std::cout << PQfname(res, j) << ": " << PQgetvalue(res, i, j) << std::endl;
        }
        std::cout << "--------------------------" << std::endl;
    }

    // 清理结果
    PQclear(res);
}

// 错误处理函数，关闭连接并退出
void exit_nicely(PGconn *conn) {
    PQfinish(conn);
    exit(1);
}
void testInsert(pair<string,string> kv) {
    string conninfo = PGSQL_CONNINFO;
    PGconn *conn = PQconnectdb(conninfo.c_str());

    // 检查连接状态
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
        exit_nicely(conn);
    }

    // 准备 SQL 语句
    const char *stmtName = "insert_statement";
    const char *sql = "INSERT INTO kvtest (key, value) VALUES ($1, $2);";

    // 预处理 SQL 语句
    PGresult *res = PQprepare(conn, stmtName, sql, 2, nullptr);

    // 检查预处理结果状态
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Preparation of insert statement failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        exit_nicely(conn);
    }
    PQclear(res);  // 清理预处理结果

    // 要插入的参数值
    const char *paramValues[2];
    paramValues[0] = kv.first.c_str();  // 第一个参数值
    paramValues[1] = kv.second.c_str();  // 第二个参数值

    // 执行参数化插入操作
    res = PQexecPrepared(conn, stmtName, 2, paramValues, nullptr, nullptr, 0);

    // 检查插入操作结果状态
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "INSERT failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        exit_nicely(conn);
    }

    // 成功插入后输出提示
    std::cout << "Data inserted successfully!" << std::endl;

    // 清理结果
    PQclear(res);

    // 关闭连接
    PQfinish(conn);
}

int main() {
    vector<vector<string>> tables = {{"czt","81","Good"},{"zhg","81","Goode"}};
    RowMultiMap mmp = DataMapper::rowMapperConstruct(0,tables);

    EncryptManager encrypt_manager = EncryptManager();
    EncryptedMultiMap emm = encrypt_manager.setup(mmp);

    vector<string> keys = emm.getKeys();
    for(auto key : keys) {
        testInsert(pair<string,string>(key,emm.get(key)));
    }


    testAES();
    return 0;
}
