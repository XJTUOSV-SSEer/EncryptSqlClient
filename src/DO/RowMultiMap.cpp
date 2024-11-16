//
// Created by 陈志庭 on 24-9-25.
//

#include "RowMultiMap.h"

#include <utility>

RowMultiMap::RowMultiMap() {
    this->maps.empty();
    this->ciperTextLenMap.empty();
    this->columnsTypes.empty();
};


void RowMultiMap::add(const string& index, vector<string>& inputData,vector<int> cipertextLength) {
    if(keys.count(index)){
        for(int i=0;i<inputData.size();i++){
            maps[index].push_back(inputData[i]);
            ciperTextLenMap[index].push_back(cipertextLength[i]);
        }
    }
    else {
        keys.insert(index);
        maps[index] = inputData;
        ciperTextLenMap[index] = std::move(cipertextLength);
    }
}

void RowMultiMap::add(pair<int,int> p_index, vector<string> inputData,vector<int> cipertextLength) {
    string index= std::to_string(p_index.first) + "," + std::to_string(p_index.second);
    keys.insert(index);
    maps[index] = std::move(inputData);
    ciperTextLenMap[index] = std::move(cipertextLength);
}

vector<string> RowMultiMap::get(const string& index) {
    vector<string> result = maps[index];
    return result;
}

vector<string> RowMultiMap::get(pair<int,int> p_index) {

    string index = std::to_string(p_index.first) + "," + std::to_string(p_index.second);
    vector<string> result = maps[index];
    return result;
}
vector<int> RowMultiMap::getCipertextLength(const string& index) {
    vector<int> result = ciperTextLenMap[index];
    return result;
}
vector<int> RowMultiMap::getCipertextLength(pair<int,int> p_index) {
    string index = std::to_string(p_index.first) + "," + std::to_string(p_index.second);
    vector<int> result = ciperTextLenMap[index];
    return result;
}

vector<string> RowMultiMap::getKeys() {
    vector<string> result;
    result.assign(keys.begin(), keys.end());
    return result;
}

bool RowMultiMap::isEmpty() {
    return maps.empty(); }

void RowMultiMap::setColumnsTypes(vector<string> columnsTypes){
    this->columnsTypes = std::move(columnsTypes); }
string RowMultiMap::getTypesByColumnsID(int columnsID){
    return columnsTypes[columnsID];
}


