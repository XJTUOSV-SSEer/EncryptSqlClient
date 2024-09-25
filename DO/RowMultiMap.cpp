//
// Created by 陈志庭 on 24-9-25.
//

#include "RowMultiMap.h"

#include <utility>

RowMultiMap::RowMultiMap() {
    this->maps.empty();
    this->columnsIndex.empty();
};

void RowMultiMap::add(const string& index, vector<string>& inputData) {
    keys.insert(index);
    maps[index] = inputData;
}

void RowMultiMap::add(pair<int,int> p_index, vector<string> inputData) {
    string index= std::to_string(p_index.first) + "," + std::to_string(p_index.second);
    keys.insert(index);
    maps[index] = std::move(inputData);
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

set<string> RowMultiMap::getKeys() {
    return this->keys;
}
