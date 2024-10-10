//
// Created by 陈志庭 on 24-9-21.
//

#include "DataMapper.h"
#include "EncryptTools/Crypto_Primitives.h"
#include "EncryptTools/EncryptUtil.h"
#include <iostream>
#include<string>
#include<vector>
#include<fstream>
#include<sstream>

#include "main.h"
#include "DO/RowMultiMap.h"
using namespace std;


RowMultiMap DataMapper::rowMapperConstruct(int tableID, vector<vector<string> > inData) {
	RowMultiMap mmap;
	const int row_size = inData.size();
	const int col_size = inData[0].size();

	int row_index = 0, col_index = 0;
	for(;row_index < row_size; row_index++) {
		pair index(tableID,row_index);
		vector<int> row_text_len;
		vector<string> row;
		for(col_index = 0; col_index < col_size; col_index++) {
			string text = inData[row_index][col_index];
			auto* plaintext = padding16(text);
			int textlen  = strlen(reinterpret_cast<char*>(plaintext));
			auto* ciphertext = new unsigned char[textlen];

			string MMType = "row";
			string key =DATA_KEY_1;
			string iv = DATA_IV_1;

			int cipertext_len = Crypto_Primitives::sym_encrypt(plaintext,textlen*8,StringToUchar(key),StringToUchar(iv),ciphertext);
			row.emplace_back(reinterpret_cast<char*>(ciphertext),cipertext_len);
			row_text_len.emplace_back(cipertext_len);
		}
		mmap.add(index,row,row_text_len);
	}
	return mmap;
}
vector<vector<string>> DataMapper::rowMapperDecrypt(RowMultiMap rmm) {
	string enc_key =DATA_KEY_1;
	string enc_iv =DATA_IV_1;

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
			Crypto_Primitives::sym_decrypt(StringToUchar(text),textlen,StringToUchar(enc_key),StringToUchar(enc_iv),plaintext);
			res_row.emplace_back(reinterpret_cast<char*>(plaintext),textlen);
		}
		result.emplace_back(res_row);
	}
	return result;
}



vector<vector<string>> DataMapper::fileReader(const string& fileName) {
    ifstream inFile;
	inFile.open(fileName);
    string lineStr;
    vector<vector<string>> strArray;
	getline(inFile, lineStr);
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