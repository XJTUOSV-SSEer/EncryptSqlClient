//
// Created by 陈志庭 on 24-12-12.
//

#ifndef TABLEINFO_H
#define TABLEINFO_H

#include "dataObject.h"
using namespace std;


class TableInfo {
private:
    string name;
    vector<string> columns;
    vector<string> columns_type;
    vector<string> key_type;
public:
    TableInfo();
    void load_from_table(Table table);
    void set_columns(vector<string> columns);
    void set_columns_type(vector<string> columns_type);
    void set_name(string name);
    void set_key_type(vector<string> key_type);
    string get_name();
    vector<string> get_columns();
    vector<string> get_columns_type();
    vector<string> get_key_type();
};



#endif //TABLEINFO_H
