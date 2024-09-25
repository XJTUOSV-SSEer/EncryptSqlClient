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
using namespace std;


map<string, vector<string>> DataMapper::rowMapperConstruct(int tableID, vector<vector<string> > inData) {
	map<string, vector<string>> map;
	const int row_size = inData.size();
	const int
			col_size = inData[0].size();

	int row_index = 0, col_index = 0;
	for(;col_index < col_size; col_index++) {
		for(row_index = 0; row_index < row_size; row_index++) {
			int str_size = inData[row_index][col_index].size();
			int plaintext_len = str_size - str_size % 16 + 16;

			auto* plaintext = new unsigned char[str_size];
			Crypto_Primitives::string2char(inData[row_index][col_index],plaintext);

			string key = KeyGenerator(32);
			string iv  = KeyGenerator(16);
			unsigned char* ciphertext = new unsigned char[plaintext_len];

			Crypto_Primitives::sym_encrypt(plaintext, plaintext_len, StringToUchar(key),
			StringToUchar(iv), ciphertext);


		}
	}
	return map;
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