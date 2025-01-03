//
// Created by 陈志庭 on 24-12-12.
//

#include "TableInfo.h"

#include <utility>

TableInfo::TableInfo() {}
void TableInfo::load_from_table(Table table) {
    name = table.get_name();
    columns = table.get_columns();
    columns_type = table.get_columns_type();
    key_type = table.get_key_type();
}

void TableInfo::set_columns(vector<string> columns) {
    this->columns = std::move(columns);
}
void TableInfo::set_columns_type(vector<string> columns_type) {
    this->columns_type = std::move(columns_type);
}
void TableInfo::set_name(string name) {
    this->name = std::move(name);
}
void TableInfo::set_key_type(vector<string> key_type) {
    this->key_type = std::move(key_type);
}

string TableInfo::get_name() {
    return this->name;
}
vector<string> TableInfo::get_columns() {
    return this->columns;
}
vector<string> TableInfo::get_columns_type() {
    return this->columns_type;
}
vector<string> TableInfo::get_key_type() { return this->key_type; }
