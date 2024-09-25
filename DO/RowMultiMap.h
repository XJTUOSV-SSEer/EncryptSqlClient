//
// Created by 陈志庭 on 24-9-25.
//

#ifndef MULTIMAP_H
#define MULTIMAP_H
#include <map>
#include <set>
#include <vector>
using namespace std;

/**
 * 实现多映射结构，名字加上 Row 以便与自带的 multimap 区分。
 */
class RowMultiMap {
private:
    /**
     * 用于补充列名，暂时用不到
     */
    map<string ,int> columnsIndex;
    /**
     * 存放 MM 结构，此处采用 map 和 vector 实现
     */
    map<string,vector<string>> maps;
    /**
     * 维护一个所有 key 也就是索引信息的集合，用于遍历。
     */
    set<string>keys;

public:
    RowMultiMap();

    /**
     * 向内部的 MM 添加条目信息
     * @param index 索引，格式为 i,j
     * @param inputData 行向量，记录数据库中每行的的信息
     */
    void add(const string& index, vector<string>& inputData);
 /**
     * 向内部的 MM 添加条目信息
     * @param p_index 索引，格式为 pair<int,int>(i,j)
     * @param inputData 行向量，记录数据库中每行的的信息
     */
    void add(pair<int,int> p_index, vector<string> inputData);

    /**
     * 利用索引从 MM 中获得行向量信息
     * @param index 索引字符串
     * @return 行向量
     */
    vector<string> get(const string& index);
    /**
     * 利用索引从 MM 中获得行向量信息
     * @param index 索引pair
     * @return 行向量
     */
    vector<string> get(pair<int,int> index);

    /**
     * 获得所有索引
     * @return 索引向量
     */
    set<string> getKeys();

};


#endif //MULTIMAP_H
