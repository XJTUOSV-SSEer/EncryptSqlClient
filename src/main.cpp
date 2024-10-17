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
#include <iostream>
#include <libpq-fe.h>
#include <vector>
#include "base64/base64.h"
#include <cstdio>
#include<gmpxx.h>
#include<gmp.h>

#include <pybind11/embed.h> // 用于嵌入 Python 解释器
namespace py = pybind11;
using namespace std;
using namespace seal;

// 函数：将 std::string 转换为 std::vector<char>
void stringToChar(string str,char* output) {
    for(int i=0;i<str.size();i++) {
        output[i] = str[i];
    }
}

string testQuery1(string key,PGconn *conn) {

    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "连接数据库失败: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return nullptr;
    } else {
        std::cout << "已成功连接到数据库！\n";
    }

    // 执行查询
    const char *query = R"(SELECT value_bytea FROM kvtest WHERE "key_bytea" = $1;)";
    const char *paramValues[1];
    int paramLengths[1];
    int paramFormats[1];

    // 将参数转换为字符串（文本格式）
    char* key_char =new char[key.size()+1];
    stringToChar(key,key_char);
    paramValues[0] = key_char;
    paramLengths[0] = 0;// 对于文本格式参数，长度可以为 0
    paramFormats[0] = 0; // 0 表示文本格式

    // 执行参数化查询
    //PGresult *res = PQexec(conn, query);
    PGresult *res = PQexecParams(conn,
                               query,
                               1,           // 参数个数
                               nullptr,        // 参数类型 OIDs，NULL 表示让服务器自行推断
                                 paramValues, // 参数值
                                 paramLengths, // 参数长度
                                 paramFormats, // 参数格式
                                 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "执行查询失败: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        PQfinish(conn);
        return nullptr;
    }


    string::size_type len;
    unsigned char *data = PQunescapeBytea((unsigned char*)PQgetvalue(res, 0, 0), (size_t*)&len);
    //string data_str = UcharToString(data,len);
    if (data == NULL) {
        cerr << "未找到数据" << endl;
        delete[] data;

        PQclear(res);

        return nullptr;
    }
    return  {reinterpret_cast<const char*>(data), len};
    // 输出数据



    // 如果需要将 bytea 数据保存为文件
    /*
    std::ofstream outfile("output.bin", std::ios::binary);
    outfile.write(reinterpret_cast<const char*>(data), len);
    outfile.close();
    */
    delete[] data;
    delete[] key_char;
    // 释放解码后的数据
    PQfreemem(data);

    // 清理资源
    PQclear(res);


}


void testSeal() {
    stringstream ss;

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
    int value1 = 42;
    int value2 = 32;
    Plaintext x_plain1(uint64_to_hex_string(value1));
    Plaintext x_plain2(uint64_to_hex_string(value2));

    // 加密
    Ciphertext encrypted_v1,encrypted_v2;
    encryptor.encrypt(x_plain1, encrypted_v1);
    encryptor.encrypt(x_plain2, encrypted_v2);
    //cout << "Encrypted message size: " << encrypted.size() << endl;

    //相加
    Ciphertext encrypted_v3;
    evaluator.add(encrypted_v1,encrypted_v2,encrypted_v3);
    encrypted_v3.save(ss);

    string viewSS = ss.str();
    cout << "CipherText's size is " <<viewSS.size() << endl;
    // 解密
    Plaintext decrypted_plaintext;
    decryptor.decrypt(encrypted_v3,decrypted_plaintext);
    int res = hexStringToInt(decrypted_plaintext.to_string());
    // 解码
    cout << "Decrypted value:" << res << '\n' <<endl;


}
//TODO 实现对 Python 脚本的调用，以备不时之需。
// void testPythonScript() {
//	py::module_ script = py::module_::import("pythonScripts.scriptZPaillier"); // 导入 Python 脚本
//	py::object result = script.attr("addInPlaceReturnPlainValue")(5, 3);        // 调用函数
//	int sum = result.cast<int>();                        // 转换结果
//	std::cout << "结果是：" << sum << std::endl;
//}
void exit_nicely(PGconn *conn) {
    PQfinish(conn);
    exit(1);
}
void testInsert(const pair<string,string>& kv,PGconn *conn) {


    string key = kv.first;
    string val = kv.second;



    int key_len = key.size();
    int val_len = val.size();

    char *key_copy = new char[key_len+1];
    char *val_copy = new char[val_len+1];

    stringToChar(key,key_copy);
    stringToChar(val,val_copy);

    const char *paramValues_1[2] = {key_copy,val_copy};
    int paramLengths_1[2] = {0,val_len};
    int paramFormats_1[2] = {0,1};

    //const char *paramValues_2[2] = {val_copy,key_copy};
    //int paramLengths_2[1] = {0};
    //int paramFormats_2[1] = {0};

    const char *sql = "INSERT INTO kvtest (key_bytea,value_bytea) VALUES ($1,$2);";
    const char *sql2 = "update kvtest set value_bytea = $1 where key_bytea = $2";

    PGresult *res = PQexecParams(conn,
                                 sql,
                                 2,           // 参数个数
                                 NULL,        // 参数类型 OIDs，NULL 表示让服务器自行推断
                                 paramValues_1, // 参数值
                                 paramLengths_1,// 参数长度
                                 paramFormats_1,
                                 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "执行key插入操作失败: " << PQerrorMessage(conn) << std::endl;
    } else {
        std::cout << "数据key插入成功！\n";
    }
    PQclear(res);

}

void testFull() {
    EncryptionParameters parms(scheme_type::bfv);

    // 设置多项式的模数（多项式环的大小），必须为2的幂次方
    size_t poly_modulus_degree = 2048;
    parms.set_poly_modulus_degree(poly_modulus_degree);

    // 设置系数模数
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));

    // 设置纯文本模数（加密运算的模数）
    parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));
    /*
     * 测试一下加密函数
     */
    //testAES();
    DataMapper data_mapper(parms);

    /*
     * 测试一下 Setup 函数
     */
    vector<string> types = {"string","int","string"};
    vector<vector<string>> tables = {{"czt","81","Good"},{"zhg","84","Goode"}};
    RowMultiMap mm = data_mapper.rowMultiMapConstruct(0,tables,types);
    //
    map<string,string> index_to_keys;
    EncryptManager encrypt_manager = EncryptManager();
    EncryptedMultiMap emm = encrypt_manager.setup(mm,index_to_keys);
    //
    string key = index_to_keys["0,0,1"];
    vector<string> keys = emm.getKeys();
    string conninfo = PGSQL_CONNINFO;
    PGconn *conn = PQconnectdb(conninfo.c_str());
    //检查连接状态
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection to database failed: " << PQerrorMessage(conn) << std::endl;
        exit_nicely(conn);
    }
    for(auto key : keys) {
        testInsert(pair<string,string>(key,emm.get(key)),conn);
    }
    string viewKey = index_to_keys["0,0,1"];
    string cipherData = testQuery1(index_to_keys["0,0,1"],conn);
    int view = data_mapper.decryptData(cipherData);
    cout << view << endl;

    //关闭连接
    PQfinish(conn);
}

int main() {
   testFull();

    //testSeal();
    //testPaillier();
	//testPythonScript();


    return 0;
}
