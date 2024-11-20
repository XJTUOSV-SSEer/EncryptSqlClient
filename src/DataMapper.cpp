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

#include "DO/RowMultiMap.h"
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



void DataMapper::insertIntoRowBySymmetricEncryption(vector<string> &row, vector<int> &row_text_len,const string& text){
    string MMType = "row";
    string key =DATA_KEY_1;
    string iv = DATA_IV_1;

    int padLength = text.size();
    if(text.size() % 16 != 0) {
        padLength = text.size()-text.size() % 16 +16;
    }


    auto* plain_text = new unsigned char[padLength];
    auto* ciphertext = new unsigned char[padLength + 16];
    auto* key_uc = new unsigned char[key.size()];
    auto* iv_uc = new unsigned char[iv.size()];

    StringToUchar(key,key_uc);
    StringToUchar(iv,iv_uc);
    //StringToUchar(text,plain_text);

    padding16(text,plain_text);
    int cipertext_len = Crypto_Primitives::sym_encrypt(plain_text,padLength,key_uc,iv_uc,ciphertext);
    //string cipherStr = UcharToString(ciphertext,cipertext_len);
    string cipherStr = string(reinterpret_cast<const char*>(ciphertext), cipertext_len);
    row.push_back(cipherStr);
    row_text_len.push_back(cipertext_len);

    delete[] plain_text;
    delete[] key_uc;
    delete[] iv_uc;
}
void DataMapper::insertIntoRowByPrfEncryption(vector<string> &row, vector<int> &row_text_len,const string& text){
    string MMType = "row";
    string key =DATA_KEY_1;
    string iv = DATA_IV_1;

    int padLength = text.size();
    if(text.size() % 16 != 0) {
        padLength = text.size()-text.size() % 16 +16;
    }


    auto* plain_text = new unsigned char[padLength];
    auto* ciphertext = new unsigned char[padLength + 16];
    auto* key_uc = new unsigned char[key.size()];
    auto* iv_uc = new unsigned char[iv.size()];

    StringToUchar(key,key_uc);
    StringToUchar(iv,iv_uc);
    //StringToUchar(text,plain_text);

    // 利用 prf 生成 F(index)
    unsigned char* enc_idx = new unsigned char[padLength];
    int cipertext_len = prfFunctionReturnUnsignedChar(text, enc_idx);
    string cipherStr = unsignedCharArrayToHexString(enc_idx, cipertext_len);

    //string cipherStr = UcharToString(enc_idx,cipertext_len);

    row.push_back(cipherStr);
    row_text_len.push_back(cipertext_len);

    delete[] plain_text;
    delete[] key_uc;
    delete[] iv_uc;
}

void DataMapper::insertIntoRowByHomomorphicEncryption(vector<string> &row, vector<int> &row_text_len, int value){
    stringstream ss;
    string viewPlainText = intToHexString(value);
    Plaintext plaintext(intToHexString(value));
    Ciphertext ciphertext;
    encryptor.encrypt(plaintext,ciphertext);

    ciphertext.save(ss);

    row.push_back(ss.str());
    row_text_len.push_back(ss.str().size());

    ss.clear();
}



RowMultiMap DataMapper::rowMultiMapConstruct(int tableID, vector<vector<string> > inData,vector<string> columnsTypes) {
	RowMultiMap mmap;
	mmap.setColumnsTypes(columnsTypes);
	const int row_size = inData.size();
	const int col_size = inData[0].size();

	int row_index = 0, col_index = 0;
	for(;row_index < row_size; row_index++) {
		pair index(tableID,row_index);
	    // 分别用于标识每行数据和对应长度的向量
	    vector<string> row;
		vector<int> row_text_len;
        vector<string> row_type;
		for(col_index = 0; col_index < col_size; col_index++) {
			string text = inData[row_index][col_index];
		    string type = mmap.getTypesByColumnsID(col_index);

		    if(type == "string") {
		        insertIntoRowBySymmetricEncryption(row, row_text_len, text);


            } else if(type == "int") {
		        insertIntoRowByHomomorphicEncryption(row, row_text_len, stoi(text));
            }
            row_type.push_back(type);

		}
		mmap.add(index,row,row_text_len);
	}
	return mmap;
}

RowMultiMap DataMapper::colMultiMapConstruct(int tableID, vector<vector<string> > inData,vector<string> columnsTypes) {
    RowMultiMap mmap;
    mmap.setColumnsTypes(columnsTypes);
    const int row_size = inData.size();
    const int col_size = inData[0].size();

    int row_index = 0, col_index = 0;
    for(;col_index < col_size; col_index++) {
        pair index(tableID,col_index);
        // 分别用于标识每列数据和对应长度的向量
        vector<string> col;
        vector<int> col_text_len;

        for(row_index = 0; row_index < row_size; row_index++) {
            string text = inData[row_index][col_index];
            string type = mmap.getTypesByColumnsID(col_index);

            if(type == "string") {
                insertIntoRowBySymmetricEncryption(col, col_text_len, text);

            } else if(type == "int") {
                insertIntoRowByHomomorphicEncryption(col, col_text_len, stoi(text));
            }

        }
        mmap.add(index,col,col_text_len);
    }
    return mmap;
}
RowMultiMap DataMapper::valueMultiMapConstruct(int tableID, vector<vector<string>> inData,
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
            vector<int> row_text_len;
            string text = inData[row_index][col_index];

            ss << tableID << "," << col_index << "," << text;
            string index  = ss.str();
            ss.str("");

            ss << tableID << "," << row_index;
            string valuePlainText = ss.str();
            ss.str("");

            insertIntoRowByPrfEncryption(row,row_text_len,valuePlainText);

            mmap.add(index,row,row_text_len);
        }

    }
    return mmap;
}
RowMultiMap DataMapper::joinMultiMapConstruct(int tableID1, int tableID2, vector<vector<string>> table1,
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

                ss << tableID1 << "," << r1;
                string index = ss.str();
                ss.str("");

                ss << tableID2 << "," << r2;
                string value = ss.str();
                ss.str("");

                insertIntoRowByPrfEncryption(row,row_text_len,value);

                mmap.add(index,row,row_text_len);
            }
        }
    }

    return mmap;
}
vector<vector<string>> DataMapper::rowMapperDecrypt(RowMultiMap rmm) {
	string enc_key =DATA_KEY_1;
	string enc_iv =DATA_IV_1;

    auto* enc_key_uc = new unsigned char[enc_key.size()];
    auto* enc_iv_uc = new unsigned char[enc_iv.size()];

    StringToUchar(enc_key,enc_key_uc);
    StringToUchar(enc_iv,enc_iv_uc);

	vector<vector<string>> result;
	vector<string> keys = rmm.getKeys();
	for(auto key : keys) {
		vector<string> res_row;
		vector<string> row = rmm.get(key);
		vector<int> text_len = rmm.getCipertextLength(key);
		for(int i=0;i<row.size();i++) {
			int textlen = text_len[i];
			string text = row[i];
			auto* plaintext = new unsigned char[textlen];
		    auto* ciphertext = new unsigned char[textlen];

            StringToUchar(text,ciphertext);

			Crypto_Primitives::sym_decrypt(ciphertext,textlen,enc_key_uc,enc_iv_uc,plaintext);
			res_row.push_back(reinterpret_cast<char*>(plaintext));

		    delete[] plaintext;
		    delete[] ciphertext;
		}
		result.push_back(res_row);
	}
    delete[] enc_key_uc;
    delete[] enc_iv_uc;
	return result;
}



vector<vector<string>> DataMapper::fileReader(const string& fileName) {
    ifstream inFile;
	inFile.open(fileName);
    string lineStr;
    vector<vector<string>> strArray;
	//getline(inFile, lineStr);
    while (getline(inFile, lineStr)) {
      cout << lineStr << endl;
      stringstream ss(lineStr);
      string str;
      vector<string> lineArray;
      while (getline(ss, str, ',')){
  		lineArray.push_back(str);
  	}
      strArray.push_back(lineArray);
    }
    return strArray;
                        
}
void DataMapper::generateEmmIntoSql(PGconn *conn,int tableID, vector<vector<string>> table, vector<string> types) {




    //cout << "从数据源中读入数据:"<< data_src << endl;
    //vector<vector<string>> tables = data_mapper.fileReader(data_src);
    //建立 mm，默认表号为 0
    RowMultiMap mmr = rowMultiMapConstruct(tableID,table,types);
    RowMultiMap mmc = colMultiMapConstruct(tableID,table,types);
    RowMultiMap mmv = valueMultiMapConstruct(tableID,table,types);

    EncryptedMultiMap emmr = EncryptManager::setupPerRow(mmr,false);
    EncryptedMultiMap emmv = EncryptManager::setupPerRow(mmv,false);
    EncryptedMultiMap emmc = EncryptManager::setupPerRow(mmc,false);


    //准备执行插入
    insertEMM(emmr,conn,"mmr",true);
    insertEMM(emmv,conn,"mmv",false);
    insertEMM(emmc,conn,"mmc",true);
}

void DataMapper::generateJoinEmmIntoSql(PGconn *conn,int tableID1,int tableID2,
    vector<vector<string>> table1,vector<vector<string>>table2,int joinCol1,int joinCol2) {

    stringstream ss;

    RowMultiMap mmjoin = joinMultiMapConstruct(tableID1,tableID2, table1, table2, joinCol1,joinCol2);
    EncryptedMultiMap emmjoin = EncryptManager::setupPerRow(mmjoin,false);

    ss << "mmjoin_" << tableID1<< "_" << joinCol1 << "_join_"
    << tableID2 << "_" << joinCol2;
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
    PGresult *res = PQexec(conn, query);


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




