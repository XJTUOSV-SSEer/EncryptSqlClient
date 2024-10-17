//
// Created by 陈志庭 on 24-10-10.
//
#include <gmp.h>


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
//void testAES() {
//    string text = "Hello World!";
//    auto* plaintext = padding16(text);
//    int textlen  = strlen(reinterpret_cast<char*>(plaintext));
//    auto* ciphertext = new unsigned char[textlen];
//    auto* plaintext2 = new unsigned char[textlen];
//    pair index(0,0);
//    string MMType = "row";
//    string key =KeyGeneratorForCP(32);
//    string iv = KeyGeneratorForCP(16);
//    EncKey ek = EncKey(index,key,iv,MMType);
//
//    unsigned char ciphertext2[textlen];
//
//    int cipertext_len = Crypto_Primitives::sym_encrypt(plaintext,textlen*8,StringToUchar(key),StringToUchar(iv),ciphertext);
//    Crypto_Primitives::sym_decrypt(ciphertext,cipertext_len,StringToUchar(key),StringToUchar(iv),plaintext2);
//
//    cout << plaintext2 << endl;
//
//}
/**
 * 测试 prf 随机生成函数。
 */
//void testSetup() {
//    string str_key = DATA_KEY_2;
//    unsigned char* key = StringToUchar(str_key);
//    string ori_key = "0,1";
//    string full_index = ori_key + ',' + to_string(0);
//    int key_size = full_index.size() * 8;
//    auto* encrypted_key = new unsigned char[full_index.size()];
//    Crypto_Primitives::get_prf(key,StringToUchar(full_index),key_size,encrypted_key);
//
//    printf("enc_key = %s\n",reinterpret_cast<char*>(encrypted_key));
//}
void testPaillier() {
    clock_t time=clock();
	 gmp_randstate_t grt;
	 gmp_randinit_default(grt);
	 gmp_randseed_ui(grt, time);

	 //p、q初始化
	 mpz_t p,q,p1,q1;

	 mpz_init(p);
	 mpz_init(q);
	 mpz_init(p1);
	 mpz_init(q1);

	 //p、q的的范围在0~2^128-1
	 mpz_urandomb(p, grt, 128);
	 mpz_urandomb(q, grt, 128);

	  //生成p,q大素数
	 mpz_nextprime(p, p);
	 mpz_nextprime(q, q);

	 //求p，q的乘积 n,以及n的平方n2
	 mpz_t n,n2;

	 mpz_init(n);
	 mpz_init(n2);
	 mpz_mul(n,p,q);
	 mpz_mul(n2,n,n);

	 //设置g,取值g=n+1
	 mpz_t g,j;

	 mpz_init(g);
	 mpz_init_set_ui(j,1);
	 mpz_urandomb(g, grt, 128);
	 //mpz_add(g,n,j);

	 //设置明文m
	 mpz_t m,m1;
	 mpz_init_set_str(m,"22",10);
	 mpz_init_set_str(m1,"33",10);
	 mpz_t r;//设置r,r为随机数
	 mpz_urandomb(r, grt, 128);

	 //设置密文c,c1,需要对这两个密文做同态加法
	 mpz_t c,c1;

	 mpz_init(c);
	 mpz_init(c1);
	 //设置密文c

	 mpz_powm(c,g,m,n2);
	 mpz_powm(r,r,n,n2);
	 mpz_mul(c,c,r);
	 mpz_mod(c,c,n2);

	 //设置密文c1
	 mpz_powm(c1,g,m1,n2);
	 mpz_mul(c1,c1,r);
	 mpz_mod(c1,c1,n2);

	 //解密过程
	 //先求λ，是p、q的最小公倍数,y3代表λ
	 mpz_t y1,y2,y3;

	 mpz_init(y1);
	 mpz_init(y2);
	 mpz_init(y3);

	 mpz_sub(p1,p,j);
	 mpz_sub(q1,q,j);
	 mpz_lcm(y3,p1,q1);//y3代表λ

	 //输出明文m,g
	 //十进制输出是%Zd,十六进制输出是%ZX,folat使用&Ff
	 //gmp_printf("p = %Zd\n\n", p);
	 //gmp_printf("q = %Zd\n\n", q);
	 //gmp_printf("r = %Zd\n\n", r);
	 //gmp_printf("g = %Zd\n\n", g);
	 //gmp_printf("λ = %Zd\n\n", y3);
	 //输出密文
	 gmp_printf("明文m = %Zd\n\n", m);
	 gmp_printf("密文c = %Zd\n\n",c);
	 gmp_printf("明文m1 = %Zd\n\n", m1);
	 gmp_printf("密文c = %Zd\n\n",c1);

	 //两个密文做同态加法:密文做乘法，最后解密是明文做加法
	 mpz_mul(c,c,c1);
	 mpz_mod(c,c,n2);

	 //y1代表c的λ次方摸n平方
	 mpz_powm(y1,c,y3,n2);
	 mpz_sub(y1,y1,j);
	 mpz_div(y1,y1,n);

	 //y2代表g的λ次方摸n平方
	 mpz_powm(y2,g,y3,n2);
	 mpz_sub(y2,y2,j);
	 mpz_div(y2,y2,n);

	 mpz_t x_y;
	 mpz_init(x_y);
	 mpz_invert(x_y,y2,n);//至关重要的一步，取逆

	 mpz_mul(x_y,x_y,y1);
	 mpz_mod(x_y,x_y,n);
	 //输出明文
	 gmp_printf("解密得到明文m = %Zd\n\n",x_y);
	 mpz_clear(p);
	 mpz_clear(q);
	 mpz_clear(n);
	 mpz_clear(n2);
	 mpz_clear(p1);
	 mpz_clear(q1);
	 mpz_clear(c);
	 mpz_clear(g);
	 mpz_clear(j);
	 mpz_clear(r);
	 mpz_clear(m);
	 mpz_clear(y2);
	 mpz_clear(y1);
	 mpz_clear(y3);
	 mpz_clear(x_y);


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
//void exit_nicely(PGconn *conn) {
//    PQfinish(conn);
//    exit(1);
//}
//void testInsert(pair<string,string> kv) {
//    string conninfo = PGSQL_CONNINFO;
//    PGconn *conn = PQconnectdb(conninfo.c_str());
//
//    // 检查连接状态
//    if (PQstatus(conn) != CONNECTION_OK) {
//        std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
//        exit_nicely(conn);
//    }
//
//    string key = kv.first;
//    string val = kv.second;
//    pair<unsigned char*,int> res1 = StringToUchar2(key);
//    pair<unsigned char*,int> res2 = StringToUchar2(val);
//
//    // 准备 SQL 语句
//    unsigned char *key_uc = res1.first;
//    const unsigned char *val_uc = res2.first;
//
//    // 示例二进制数据
//
//    // 转义二进制数据，以便在 SQL 语句中使用
//    size_t escaped_data1_len;
//    size_t escaped_data2_len;
//    unsigned char *escaped_data_1 = PQescapeByteaConn(conn, key_uc, res1.second, &escaped_data1_len);
//    unsigned char *escaped_data_2 = PQescapeByteaConn(conn, val_uc, res2.second, &escaped_data2_len);
//
//    if (escaped_data_1 == nullptr || escaped_data_2 == nullptr) {
//        std::cerr << "Failed to escape binary data" << std::endl;
//        exit_nicely(conn);
//    }
//
//
//    // 准备 SQL 插入语句
//    //const char *sql =  R"(UPDATE kvHstore SET attr = attr || $1 :: hstore;)";
//    const char *sql = "INSERT INTO kv (key,value) VALUES ($1,$2);";
//    const char *paramValues[2];
//
//    // 使用参数化查询插入二进制数据
//    paramValues[0] = {reinterpret_cast<const char *>(escaped_data_1)};
//    paramValues[1] = {reinterpret_cast<const char *>(escaped_data_2)};
//
//    // 转义后的数据作为参数传递
//
//    PGresult *res = PQexecParams(conn, sql, 2, nullptr, paramValues, nullptr, nullptr, 0);
//
//    // 检查插入操作结果状态
//    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
//        std::cerr << "INSERT failed: " << PQerrorMessage(conn) << std::endl;
//        PQclear(res);
//        exit_nicely(conn);
//    }
//
//    // 成功插入后输出提示
//    std::cout << "Data inserted successfully!" << std::endl;
//
//    // 清理结果
//    PQclear(res);
//
//    // 关闭连接
//    PQfinish(conn);
//}