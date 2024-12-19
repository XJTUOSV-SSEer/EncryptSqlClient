//
// Created by 陈志庭 on 24-9-21.
//

#include "DataMapper.h"
#include <fstream>
#include <iostream>
#include <libpq-fe.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "EncryptManager.h"
#include "EncryptTools/Crypto_Primitives.h"
#include "EncryptTools/EncryptUtil.h"

#include "dataObject/dataObject.h"
#include "main.h"
#include "seal/seal.h"
using namespace std;
using namespace seal;

DataMapper::DataMapper(EncryptionParameters &parms) :
    context(parms), keygen(context),
    encryptor(context, initializePublicKey()),
    decryptor(context, getSecretKey()) {
    this->keyMap.clear();
}

PublicKey DataMapper::initializePublicKey() {
    PublicKey _public_key;
    std::ifstream public_key_file("../Resource/SealKey/public_key.seal", std::ios::binary);
    _public_key.load(context, public_key_file);
    public_key_file.close();
    return _public_key;
}

SecretKey DataMapper::getSecretKey() {
    SecretKey _secret_key;
    std::ifstream secret_key_file("../Resource/SealKey/secret_key.seal", std::ios::binary);
   _secret_key.load(context, secret_key_file);
    secret_key_file.close();
    return _secret_key;
}
PublicKey DataMapper::getPublicKey() {
    return this->public_key; }

int DataMapper::decryptData(string ciphertext){
    stringstream ss(ciphertext);
    Ciphertext ciphertext_tmp;
    Plaintext plaintext_tmp;
    ciphertext_tmp.load(context,ss);
    decryptor.decrypt(ciphertext_tmp,plaintext_tmp);

    return hexStringToInt(plaintext_tmp.to_string());



}



void DataMapper::insertIntoRowBySymmetricEncryption(vector<string> &row, const string& text){
    string MMType = "row";
    string key =DATA_KEY_1;
    string iv = DATA_IV_1;

    int padLength = text.size();
    if(text.size() % 16 != 0) {
        padLength = text.size()-text.size() % 16 +16;
    }


    string cipher_text = getSymmetricEncryption(text,false);
    int ciphertext_len = cipher_text.size();
    //string cipherStr = UcharToString(ciphertext,cipertext_len);

    row.push_back(cipher_text);


}
void DataMapper::insertIntoRowByPrfEncryption(vector<string> &row, const string& text) {
    string MMType = "row";
    string key =DATA_KEY_1;
    string iv = DATA_IV_1;


    // 利用 prf 生成 F(index)
    string cipherStr = prfFunctionReturnString(text,true);

    //string cipherStr = UcharToString(enc_idx,cipertext_len);

    row.push_back(cipherStr);
}

void DataMapper::insertIntoRowByHomomorphicEncryption(vector<string> &row, int value){
    stringstream ss;
    string viewPlainText = intToHexString(value);
    Plaintext plaintext(intToHexString(value));
    Ciphertext ciphertext;
    encryptor.encrypt(plaintext,ciphertext);

    ciphertext.save(ss);

    row.push_back(ss.str());

    ss.clear();
}



RowMultiMap DataMapper::rowMultiMapConstruct(string table_name, vector<vector<string> > inData,vector<string> columnsTypes) {
	RowMultiMap mmap;
	mmap.setColumnsTypes(columnsTypes);
	const int row_size = inData.size();
	const int col_size = inData[0].size();

	int row_index = 0, col_index = 0;
	for(;row_index < row_size; row_index++) {
		string index = table_name +","+ to_string(row_index);
	    // 分别用于标识每行数据和对应长度的向量
	    vector<string> row;
        vector<string> row_type;
		for(col_index = 0; col_index < col_size; col_index++) {
			string text = inData[row_index][col_index];
		    string type = mmap.getTypesByColumnsID(col_index);

		    if(type == "string") {
		        insertIntoRowBySymmetricEncryption(row, text);


            } else if(type == "int") {
		        insertIntoRowByHomomorphicEncryption(row, stoi(text));
            }
            row_type.push_back(type);

		}
		mmap.add(index,row);
	}
	return mmap;
}

RowMultiMap DataMapper::colMultiMapConstruct(string table_name, vector<vector<string> > inData,vector<string> columnsTypes) {
    RowMultiMap mmap;
    mmap.setColumnsTypes(columnsTypes);
    const int row_size = inData.size();
    const int col_size = inData[0].size();

    int row_index = 0, col_index = 0;
    for(;col_index < col_size; col_index++) {
        string index = table_name + ","+to_string(col_index);
        // 分别用于标识每列数据和对应长度的向量
        vector<string> col;

        for(row_index = 0; row_index < row_size; row_index++) {
            string text = inData[row_index][col_index];
            string type = mmap.getTypesByColumnsID(col_index);

            if(type == "string") {
                insertIntoRowBySymmetricEncryption(col, text);

            } else if (type == "int") {
                insertIntoRowByHomomorphicEncryption(col, stoi(text));
            }

        }
        mmap.add(index,col);
    }
    return mmap;
}
RowMultiMap DataMapper::valueMultiMapConstruct(string table_name, vector<vector<string>> inData,
                                               vector<string> columnsTypes) {
    RowMultiMap mmap;
    stringstream ss;
    mmap.setColumnsTypes(columnsTypes);
    const int row_size = inData.size();
    const int col_size = inData[0].size();

    int row_index = 0, col_index = 0;
    for(;col_index < col_size; col_index++) {



        for(row_index = 0; row_index < row_size; row_index++) {
            // 分别用于标识每列数据和对应长度的向量
            vector<string> row;
            string text = inData[row_index][col_index];

            ss << table_name << "," << col_index << "," << text;
            string index  = ss.str();
            ss.str("");

            ss << table_name << "," << row_index;
            string valuePlainText = ss.str();
            ss.str("");

            insertIntoRowByPrfEncryption(row,valuePlainText);

            mmap.add(index,row);
        }

    }
    return mmap;
}
RowMultiMap DataMapper::joinMultiMapConstruct(string table_name1, string table_name2, vector<vector<string>> table1,
                                              vector<vector<string>> table2,int joinCol1,int joinCol2) {
    stringstream ss;
    RowMultiMap mmap;
    int row_size1 = table1.size();
    int row_size2 = table2.size();
    for(int r1 = 0;r1<row_size1;r1++){
        for(int r2=0;r2<row_size2;r2++){
            string text1 = table1[r1][joinCol1];
            string text2 = table2[r2][joinCol2];

            if(text1 == text2){
                vector<string> row;
                vector<int> row_text_len;

                ss << table_name1 << "," << r1;
                string index = ss.str();
                ss.str("");

                ss << table_name2 << "," << r2;
                string value = ss.str();
                ss.str("");

                insertIntoRowByPrfEncryption(row,value);

                mmap.add(index,row);
            }
        }
    }

    return mmap;
}


Table DataMapper::fileReader(const string& fileName,bool has_header) {
    Table table;
    ifstream inFile;
	inFile.open(fileName);
    string lineStr;
    vector<vector<string>> strArray;
	//getline(inFile, lineStr);
    int count = 0;
    while (safeGetline(inFile, lineStr)) {
        if(lineStr.empty()) {
            continue;
        }
          cout << lineStr << endl;
          stringstream ss(lineStr);

          string str;
          vector<string> lineArray;
          while (getline(ss, str, ',')){
  	    	lineArray.push_back(str);
  	    }

        if(count==0 && has_header) {
            table.set_columns(lineArray);
        }
        //else if(count==1 && has_header) {
        //    table.set_key_type(lineArray);
        //}
        else if(count==1 && has_header) {
            table.set_columns_type(lineArray);
        }
        else {
            strArray.push_back(lineArray);
        }
        count++;

    }
    table.set_table(strArray);
    return table;
                        
}
void DataMapper::generateEmmIntoSql(PGconn *conn,int tableID, vector<vector<string>> table, vector<string> types) {




    //cout << "从数据源中读入数据:"<< data_src << endl;
    // vector<vector<string>> tables = data_mapper.fileReader(data_src);
    //建立 mm，默认表号为 0
    RowMultiMap mmr = rowMultiMapConstruct(to_string(tableID),table,types);
    RowMultiMap mmc = colMultiMapConstruct(to_string(tableID),table,types);
    RowMultiMap mmv = valueMultiMapConstruct(to_string(tableID),table,types);

    EncryptedMultiMap emmr = EncryptManager::setup(mmr);
    EncryptedMultiMap emmv = EncryptManager::setup(mmv);
    EncryptedMultiMap emmc = EncryptManager::setup(mmc);


    //准备执行插入
    insertEMM(emmr,conn,"mmr",true);
    insertEMM(emmv,conn,"mmv",false);
    insertEMM(emmc,conn,"mmc",true);
}

void DataMapper::generateEmmIntoSql(PGconn *conn,string table_name, vector<vector<string>> table, vector<string> types) {


    //cout << "从数据源中读入数据:"<< data_src << endl;
    //vector<vector<string>> tables = data_mapper.fileReader(data_src);
    // 建立 mm，默认表号为 0
    auto now = std::chrono::system_clock::now();
    RowMultiMap mmv = valueMultiMapConstruct(table_name,table,types);
    auto now2 = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now2 - now);
    cout << "mmv的构建时间是：" <<duration.count() << endl;
    auto now3 = std::chrono::system_clock::now();
    RowMultiMap mmr = rowMultiMapConstruct(table_name,table,types);
    auto now4 = std::chrono::system_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(now4 - now3);
    cout << "mmr的构建时间是："<< duration.count() << endl;

    RowMultiMap mmc = colMultiMapConstruct(table_name,table,types);


    auto now5 = std::chrono::system_clock::now();
    EncryptedMultiMap emmr = EncryptManager::setup(mmr);
    auto now6 = std::chrono::system_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(now6 - now5);
    cout << "emmr的构建时间是：" <<duration.count() << endl;
    auto now7 = std::chrono::system_clock::now();
    EncryptedMultiMap emmv = EncryptManager::setup(mmv);
    auto now8 = std::chrono::system_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(now8 - now7);
    cout << "emmv的构建时间是："<< duration.count() << endl;
    auto now9 = std::chrono::system_clock::now();
    EncryptedMultiMap emmc = EncryptManager::setup(mmc);
    auto now10 = std::chrono::system_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(now10 - now9);
    cout << "emmc的构建时间是："<< duration.count() << endl;


    //准备执行插入
    insertEMM(emmr,conn,"mmr",true);
    insertEMM(emmv,conn,"mmv",false);
    insertEMM(emmc,conn,"mmc",true);
}


void DataMapper::generateJoinEmmIntoSql(PGconn *conn, string table_name1, string table_name2,
                                        vector<vector<string>> table1,vector<vector<string>>table2,int joinCol1,int joinCol2) {

    stringstream ss;

    RowMultiMap mmjoin = joinMultiMapConstruct(table_name1,table_name2, table1, table2, joinCol1,joinCol2);
    EncryptedMultiMap emmjoin = EncryptManager::setup(mmjoin);

    ss << "mmjoin_" << table_name1<< "_" << joinCol1 << "_join_"
    << table_name2<< "_" << joinCol2;
    //准备执行插入
    insertEMM(emmjoin,conn,ss.str(),false);
}

void DataMapper::generateJoinEmmIntoSql(PGconn *conn, int tableID1, int tableID2, vector<vector<string>> table1,vector<vector<string>>table2,int joinCol1,int joinCol2) {

    stringstream ss;

    RowMultiMap mmjoin = joinMultiMapConstruct(to_string(tableID1),to_string(tableID2), table1, table2, joinCol1,joinCol2);
    EncryptedMultiMap emmjoin = EncryptManager::setup(mmjoin);

    ss << "mmjoin_" << tableID1<< "_" << joinCol1 << "_join_"
    << tableID2<< "_" << joinCol2;
    //准备执行插入
    insertEMM(emmjoin,conn,ss.str(),false);
}

void DataMapper::insertEMM(EncryptedMultiMap emm, PGconn *conn,string targetTable,bool value_is_bytea) {
    vector<string> keys = emm.getKeys();

    createTableIfNotExsit(conn,targetTable,value_is_bytea);
    //检查连接状态
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "连接数据库失败: " << PQerrorMessage(conn) << std::endl;
        PQfinish(conn);
        return;
    }
    for(auto key : keys) {
        insertIntoSql(pair<string, string>(key, emm.get(key)),targetTable,conn,value_is_bytea);
    }

}
void DataMapper::createTableIfNotExsit(PGconn *conn,string table_name,bool value_is_bytea){
    string sqlQuery;
    if(value_is_bytea) {
        sqlQuery = "Create table if not exists " + table_name +
        " (enc_key varchar(255) PRIMARY KEY, enc_value bytea)";
    }
    else {
        sqlQuery = "Create table if not exists " + table_name +
       " (enc_key varchar(255) PRIMARY KEY, enc_value varchar(255))";
    }
    const char* query = sqlQuery.c_str();
    PQexec(conn, query);
}

void DataMapper::insertIntoSql(const pair<string,string>& kv,string targetTable,PGconn *conn,bool value_is_bytea) {
    stringstream ss;

    string key = kv.first;
    string val = kv.second;

    int key_len = key.size();
    int val_len = val.size();


    char *key_copy = new char[key_len+1];
    char *val_copy = new char[val_len+1];
    key_copy[key_len] = '\0';
    stringToChar(key,key_copy);
    stringToChar(val,val_copy);

    const char *paramValues_1[2] = {key_copy,val_copy};
    int paramLengths_1[2] = {0,val_len};
    int paramFormats_1[2] = {0,1};

    if(!value_is_bytea) {
        val_copy[val_len] = '\0';
        paramLengths_1[1] = 0;
        paramFormats_1[1] = 0;
    }


    //const char *paramValues_2[2] = {val_copy,key_copy};
    // int paramLengths_2[1] = {0};
    // int paramFormats_2[1] = {0};

    ss << "INSERT INTO "<< targetTable  << "(enc_key,enc_value) VALUES ($1,$2) ON CONFLICT(enc_key)"
                     << "DO UPDATE SET enc_value = $2 ";
    string viewSS = ss.str();
    const string::value_type *sql = viewSS.c_str();

    PGresult *res = PQexecParams(conn,
                                 sql,
                                 2,           // 参数个数
                                 NULL,        // 参数类型 OIDs，NULL 表示让服务器自行推断
                                 paramValues_1, // 参数值
                                 paramLengths_1,// 参数长度
                                 paramFormats_1,
                                 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "执行key插入操作失败: 表：" << targetTable<<" "<< PQerrorMessage(conn) << std::endl;
    }
    PQclear(res);

}

void DataMapper::load_keys_from_file(SEALContext &context,
                         PublicKey &public_key,
                         SecretKey &secret_key
                         ) {
        // 加载公钥
        std::ifstream public_key_file("./Resource/SealKey/public_key.seal", std::ios::binary);
        public_key.load(context, public_key_file);
        public_key_file.close();

        // 加载私钥
        std::ifstream secret_key_file("./Resource/SealKey/secret_key.seal", std::ios::binary);
        secret_key.load(context, secret_key_file);
        secret_key_file.close();
    }





