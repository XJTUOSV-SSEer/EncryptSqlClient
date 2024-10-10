//
// Created by 陈志庭 on 24-9-25.
//

#ifndef MULTIMAP_H
#define MULTIMAP_H
#include <map>
#include <ostream>
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
     * 存放密文长度；
     */
    map<string,vector<int>> ciperTextLenMap;
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
     * @param cipertextLength 加密后对应的密文长度
     */
    void add(const string& index, vector<string>& inputData,vector<int> cipertextLength);
 /**
     * 向内部的 MM 添加条目信息
     * @param p_index 索引，格式为 pair<int,int>(i,j)
     * @param inputData 行向量，记录数据库中每行的的信息
     */
    void add(pair<int,int> p_index, vector<string> inputData,vector<int> cipertextLength);

    /**
     * 利用索引从 MM 中获得行向量信息
     * @param index 索引字符串
     * @return 行向量
     */
    vector<string> get(const string& index);
    vector<int> getCipertextLength(const string& index);
    /**
     * 利用索引从 MM 中获得行向量信息
     * @param index 索引pair
     * @return 行向量
     */
    vector<string> get(pair<int,int> index);
    vector<int> getCipertextLength(pair<int,int> p_index);

    /**
     * 获得所有索引
     * @return 索引向量
     */
    vector<string> getKeys();
 /**
     * 重载了输出操作符
     */
 friend ostream &operator<<( ostream &output,const RowMultiMap &RM) {
  for(const auto& key:RM.keys) {
   for(vector<string> row = RM.maps.at(key); const auto& value:row) {
       output << value << endl;
    }

  }
  return output;
 }

};


#endif //MULTIMAP_H
