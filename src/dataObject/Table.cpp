//
// Created by 陈志庭 on 24-11-22.
//

#include "Table.h"

#include <utility>
Table::Table() = default;
void Table::set_table(vector<vector<string>> table){
    this->table = std::move(table);
}
void Table::set_columns(vector<string> columns){
    this->columns = std::move(columns);
}
void Table::set_columns_type(vector<string> columns_type){
    this->columns_type = std::move(columns_type);
}
void Table::set_name(std::string name) {
    this->name = std::move(name);
}
string Table::get_name() {
        return this->name;
}
vector<vector<string>> Table::get_table(){
    return this->table;
}
vector<string> Table::get_columns(){
    return this->columns;
}
vector<string> Table::get_columns_type(){
    return this->columns_type;
}