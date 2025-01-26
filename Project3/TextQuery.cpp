// TextQuery.cpp
#include "TextQuery.h"

TextQuery::TextQuery(Result &input) { // 构造函数
    searchcontent = std::move(input);
}

Result TextQuery::query(const string &word) const {
    // 构造正则表达式
    regex r(word);
    smatch result;
    Result searchres;
    for (auto i : searchcontent.nos) {
        if (regex_search(Result::input[i-1], result, r)) {
            searchres.nos.insert(i);
            searchres.times++;
        }
    }
    return searchres;
}