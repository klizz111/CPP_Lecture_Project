#pragma once
#include "searchResult.h"
#include <map>
using std::map;
#include <regex>    
using std::regex, std::regex_search, std::smatch; 

class TextQuery {
private:
    LINENO lineNo; // 行号
    Result searchcontent; // 要搜索的内容
    map<string, set<LINENO>> wordmap; // 单词与行号的映射
public:
    TextQuery() = default;
    TextQuery(Result &input); // 构造函数
    Result query(const string &word) const; // 查询
};

