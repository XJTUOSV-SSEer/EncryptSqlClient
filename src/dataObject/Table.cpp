//
// Created by 陈志庭 on 24-11-22.
//

#include "Table.h"
void Table::set_table(vector<vector<string>> table){
    this->table = table;
}
void Table::set_columns(vector<string> columns){
    this->columns = columns;
}
void Table::set_columns_type(vector<string> columns_type){
    this->columns_type = columns_type;
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