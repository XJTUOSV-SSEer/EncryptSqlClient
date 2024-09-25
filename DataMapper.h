//
// Created by 陈志庭 on 24-9-21.
//

#ifndef DATAMAPPER_H
#define DATAMAPPER_H
#include <vector>

#include <iostream>
#include<string>
#include<vector>
#include<fstream>
#include <map>
#include<sstream>
using namespace std;

/**
 * 用于判断并生成对应 MM 数据结构的类，待完成
 */
class DataMapper {

    public:
    /**
     * 构建行向量多映射结构
     * @param tableID
     * @param inData
     * @return
     */
    static map<string,vector<string>> rowMapperConstruct(int tableID,vector<vector<string>> inData);

    /**
     *
     * @param fileName
     * @return
     */
    static vector<vector<string>> fileReader( const string& fileName);
    private:
        string dataFilePath;
};



#endif //DATAMAPPER_H
