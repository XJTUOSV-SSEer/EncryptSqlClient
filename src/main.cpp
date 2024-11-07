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
#include <cstdio>
#include<gmpxx.h>
#include<gmp.h>

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
    const char *query = R"(SELECT enc_value FROM kvtest WHERE "enc_key" = $1;)";
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

//unsigned char* testQuery2(vector<string> keys,PGconn *conn,string::size_type &len) {
unsigned char* testQuery2(string key,PGconn *conn,string::size_type &len) {


    // 执行查询
    //const char *query = R"(select SUM_ex(enc_value) from kvtest where enc_key in ($1,$2,$3);)";
    const char *query = R"(select sum_by_row($1);)";

    cout << "执行 sql 语句: " << query << endl;
    const char *paramValues[1];
    int paramLengths[2]={0};
    int paramFormats[2]={0};// 0 表示文本格式

    // 将参数转换为字符串（文本格式）
    //for(int i=0;i<3;i++) {
    //    const string& key = keys[i];
    //    char* key_char =new char[key.size()+1];
    //    stringToChar(key,key_char);
    //    paramValues[i] = key_char;
    //}
    stringstream ss;

    char* key_char =new char[key.size()+1];

    stringToChar(key,key_char);
    key_char[key.size()]=0;
    paramValues[0] = key_char;


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



    unsigned char *data = PQunescapeBytea((unsigned char*)PQgetvalue(res, 0, 0), (size_t*)&len);

    //string data_str = UcharToString(data,len);
    if (data == NULL) {
        cerr << "未找到数据" << endl;
        delete[] data;

        PQclear(res);

        return nullptr;
    }
    cout << "执行查询成功" << endl;
    return  data;
    // 输出数据



    // 如果需要将 bytea 数据保存为文件
    /*
    std::ofstream outfile("output.bin", std::ios::binary);
    outfile.write(reinterpret_cast<const char*>(data), len);
    outfile.close();
    */


}
unsigned char* testQuery3(PGconn *conn,string::size_type &bytea_len) {
    // 3. 执行 SQL 查询
    const char *query = "select SUM_ex(_value) from kvtest;";
    PGresult *res = PQexec(conn, query);
    // 4. 检查查询结果
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Query failed: " << PQerrorMessage(conn) << std::endl;
        PQclear(res);
        PQfinish(conn);
    }
    // 5. 处理查询结果
    const char *bytea_data = PQgetvalue(res, 0, 0);  // 获取 bytea 数据（它是经过转义的字符串）
    // 6. 解码 bytea 数据
    unsigned char *data = PQunescapeBytea((const unsigned char *)bytea_data, &bytea_len);
    //string data_str = UcharToString(data,len);
    if (data == NULL) {
        cerr << "未找到数据" << endl;
        delete[] data;
        PQclear(res);
        return nullptr;
    }
    return  data;
    // 输出数据
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
    char *val_copy = new char[val_len];
    key_copy[key_len] = '\0';
    stringToChar(key,key_copy);
    stringToChar(val,val_copy);

    const char *paramValues_1[2] = {key_copy,val_copy};
    int paramLengths_1[2] = {0,val_len};
    int paramFormats_1[2] = {0,1};

    //const char *paramValues_2[2] = {val_copy,key_copy};
    //int paramLengths_2[1] = {0};
    //int paramFormats_2[1] = {0};

    const char *sql = "INSERT INTO kvtest (enc_key,enc_value) VALUES ($1,$2) ON CONFLICT(enc_key)"
                      "DO UPDATE SET enc_value = $2 ";
    const char *sql2 = "update kvtest set value_bytea = $1 where _key = $2";

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
    }
    PQclear(res);

}

vector<string> getKeysFromRows(string key,int key_column_no,vector<vector<string>> tables) {
    vector<string> keys;
    stringstream ss;
    int size_row = tables.size();
    int size_col = tables[0].size();
    for(int i=0;i<size_row;i++) {
        if(key == tables[i][key_column_no]) {
            for(int j=0;j<size_col;j++) {
                if(j==key_column_no)continue;
                ss << "0," << i << "," << j;
                keys.push_back(ss.str());
                ss.str("");
            }
        }
    }
    return keys;
}

vector<string> keysToEncrptedKeys(vector<string> keys,map<string,string> index_to_keys) {
    vector<string> keys_to_encrpted_keys;
    for(int i=0;i<keys.size();i++) {
        keys_to_encrpted_keys.push_back(index_to_keys[keys[i]]);
    }
    return keys_to_encrpted_keys;
}


void testSumByRow(PGconn *conn) {
    EncryptionParameters parms(scheme_type::bfv);

    // 设置 SEAL 参数
    size_t poly_modulus_degree = 2048;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
    parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));

    //设置数据路径
    string data_src = "/Users/chenzhiting/ProjectSE/Client_demo/EncryptSqlClient/src/data/data.csv";

    //读入表并生成 mm 和 emm
    DataMapper data_mapper(parms);
    vector<string> types = {"string","int","int","int"};
    //vector<vector<string>> tables = {{"张三","84","90","87"},{"李四","82","91","87"}};
    cout << "从数据源中读入数据:"<< data_src << endl;
    vector<vector<string>> tables = data_mapper.fileReader(data_src);
    //建立 mm，默认表号为 0
    RowMultiMap mm = data_mapper.colMultiMapConstruct(0,tables,types);
    map<string,string> index_to_keys;

    EncryptManager encrypt_manager = EncryptManager();
    EncryptedMultiMap emm = encrypt_manager.setupPerRow(mm);

    Evaluator evaluator(data_mapper.context);

    // 预备查询 key
    cout << "准备查询 第二列 的成绩之和" << endl;

    //准备执行插入
    vector<string> keys = emm.getKeys();
    //string conninfo = PGSQL_CONNINFO;

    //检查连接状态
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "连接数据库失败: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return;
    } else {
        std::cout << "已成功连接到数据库！\n";
    }

    int couter = 0 ;
    for(auto key : keys) {
        testInsert(pair<string,string>(key,emm.get(key)),conn);
        couter++;
    }
    cout << "数据插入成功:" << couter << endl;
    //vector<string> query_keys = keysToEncrptedKeys(getKeysFromRows(query_key,0,tables),index_to_keys);
    string query_key = prfFunctionReturnString("0,1",true);
    // 调用服务器的加法函数并返回结果

    Ciphertext ct3;
    stringstream ss;
    string::size_type len3;
    unsigned char * res_3 = testQuery2(query_key,conn,len3);
    string res_s3 = {reinterpret_cast<const char*>(res_3), len3};
    ss << res_s3;
    ct3.load(data_mapper.context,ss);


    int f_res = data_mapper.decryptData(ss.str());
    cout << "获得查询结果：" <<f_res << endl;
    PQfinish(conn);
}


int main() {
    string conninfo = PGSQL_CONNINFO;
    PGconn *conn = PQconnectdb(conninfo.c_str());
    testSumByRow(conn);
    //testFull();
    //testSeal();
    //testPaillier();
	//testPythonScript();


    return 0;
}
