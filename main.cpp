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
#include "seal/seal.h"
using namespace std;
using namespace seal;


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
    string key =KeyGeneratorForCP(32);
    string iv = KeyGeneratorForCP(16);
    EncKey ek = EncKey(index,key,iv,MMType);

    unsigned char ciphertext2[textlen];

    int cipertext_len = Crypto_Primitives::sym_encrypt(plaintext,textlen*8,StringToUchar(key),StringToUchar(iv),ciphertext);
    Crypto_Primitives::sym_decrypt(ciphertext,cipertext_len,StringToUchar(key),StringToUchar(iv),plaintext2);

    cout << plaintext2 << endl;

}
void testSeal() {
    EncryptionParameters parms(scheme_type::bfv);

    // 设置多项式的模数（多项式环的大小），必须为2的幂次方
    size_t poly_modulus_degree = 4096;
    parms.set_poly_modulus_degree(poly_modulus_degree);

    // 设置系数模数
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));

    // 设置纯文本模数（加密运算的模数）
    parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));

    // 创建SEALContext对象
    SEALContext context(parms);

    // 生成密钥
    KeyGenerator keygen(context);
    SecretKey secret_key = keygen.secret_key();
    PublicKey public_key;
    keygen.create_public_key(public_key);

    // 创建加密器、解密器和编码器
    Encryptor encryptor(context, public_key);
    Decryptor decryptor(context, secret_key);
    Evaluator evaluator(context);

    // 要加密的整数
    int value = 42;
    Plaintext x_plain(uint64_to_hex_string(value));

    // 加密
    Ciphertext encrypted;
    encryptor.encrypt(x_plain, encrypted);
    cout << "Encrypted message: " << encrypted.size() << endl;

    // 解密
    Plaintext decrypted_plaintext;
    decryptor.decrypt(encrypted, decrypted_plaintext);

    // 解码
    cout << "Decrypted value: 0x" << decrypted_plaintext.to_string() << endl;


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

    string key = kv.first;
    string val = kv.second;
    pair<unsigned char*,int> res1 = StringToUchar2(key);
    pair<unsigned char*,int> res2 = StringToUchar2(val);

    // 准备 SQL 语句
    unsigned char *key_uc = res1.first;
    const unsigned char *val_uc = res2.first;
    unsigned char *KVJoinByArrow = joinByArrow(key_uc, res1.second,val_uc,res2.second);
    // 示例二进制数据

    // 转义二进制数据，以便在 SQL 语句中使用
    size_t escaped_data1_len;
    //size_t escaped_data2_len;
    unsigned char *escaped_data_1 = PQescapeByteaConn(conn, KVJoinByArrow, res1.second + res2.second + 2, &escaped_data1_len);
    //unsigned char *escaped_data_2 = PQescapeByteaConn(conn, val_uc, res2.second, &escaped_data2_len);

    //if (escaped_data_1 == nullptr || escaped_data_2 == nullptr) {
    //    std::cerr << "Failed to escape binary data" << std::endl;
    //    exit_nicely(conn);
    //}
    if (escaped_data_1 == nullptr) {
        std::cerr << "Failed to escape binary data" << std::endl;
        exit_nicely(conn);
    }

    // 准备 SQL 插入语句
    const char *sql =  R"(UPDATE kvHstore SET attr = attr || $1 :: hstore;)";
    //const char *sql = "INSERT INTO kvHstore (title,attr) VALUES ($1,'"$2"=>"$3"');";
    const char *paramValues[1];

    // 使用参数化查询插入二进制数据
    paramValues[0] = {reinterpret_cast<const char *>(escaped_data_1)};

    // 转义后的数据作为参数传递

    PGresult *res = PQexecParams(conn, sql, 1, nullptr, paramValues, nullptr, nullptr, 0);

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
    /*
     * 测试一下加密函数
     */
    //testAES();

    /*
     * 测试一下 Setup 函数
     */

    //vector<vector<string>> tables = {{"czt","81","Good"},{"zhg","81","Goode"}};
    //RowMultiMap mm = DataMapper::rowMapperConstruct(0,tables);
//
    //EncryptManager encrypt_manager = EncryptManager();
    //EncryptedMultiMap emm = encrypt_manager.setup(mm);
//
    //vector<string> keys = emm.getKeys();
    //for(auto key : keys) {
    //    testInsert(pair<string,string>(key,emm.get(key)));
    //}

    testSeal();



    return 0;
}
