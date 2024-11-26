//
// Created by 陈志庭 on 24-11-22.
//

#ifndef TABLE_H
#define TABLE_H
#include <string>
#include <vector>
#include <sstream>
using namespace std;


class Table {
private:
    string name;
    vector<vector<string>> table;
    vector<string> columns;
    vector<string> columns_type;
public:
    Table();
    void set_table(vector<vector<string>> table);
    void set_columns(vector<string> columns);
    void set_columns_type(vector<string> columns_type);
    void set_name(string name);
    string get_name();
    vector<vector<string>> get_table();
    vector<string> get_columns();
    vector<string> get_columns_type();


};



#endif //TABLE_H
