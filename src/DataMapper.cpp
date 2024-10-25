//
// Created by 陈志庭 on 24-9-21.
//

#include "DataMapper.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
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
    keygen.create_public_key(public_key);
    return public_key;
}

SecretKey DataMapper::getSecretKey() {
    return keygen.secret_key();
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
		vector<int> row_text_len;
		vector<string> row;
		for(col_index = 0; col_index < col_size; col_index++) {
			string text = inData[row_index][col_index];
		    string type = mmap.getTypesByColumnsID(col_index);

		    if(type == "string") {
		        insertIntoRowBySymmetricEncryption(row, row_text_len, text);

            } else if(type == "int") {
		        insertIntoRowByHomomorphicEncryption(row, row_text_len, stoi(text));
		    }

		}
		mmap.add(index,row,row_text_len);
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