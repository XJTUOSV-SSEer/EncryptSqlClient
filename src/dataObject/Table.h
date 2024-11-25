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
    vector<vector<string>> table;
    vector<string> columns;
    vector<string> columns_type;
public:
    Table();
    void read_table(string file_name,bool is_firstline_header);
    void write_table(string file_name);
    void set_columns(vector<string> columns);
    void set_column_type(string column_type);

};



#endif //TABLE_H
