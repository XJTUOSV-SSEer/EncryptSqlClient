//
// Created by 陈志庭 on 24-9-25.
//

#include "EncryptedMultiMap.h"

EncryptedMultiMap::EncryptedMultiMap() {
    this->KVmap.empty();
    this->keys.empty();
};

vector<string> EncryptedMultiMap::getKeys() {
    return this->keys;
}

void EncryptedMultiMap::insert(string key, string value) {
    this->KVmap.insert(make_pair(key, value));
    this->keys.push_back(key);
}
string EncryptedMultiMap::get(string key) {
    return this->KVmap.at(key);
}

