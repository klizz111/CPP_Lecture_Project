#include <algorithm>
using std::set_intersection, std::inserter;
#include "query.h"

// 与查询
QueryResult AndQuery::eval(const TextQuery &text) const {
    QueryResult left = lhs.eval(text), right = rhs.eval(text);
    set<LINENO> ret_lines;
    // 使用set_intersection求交集
    set_intersection(left.begin(), left.end(), 
                    right.begin(), right.end(), 
                    inserter(ret_lines, ret_lines.begin())); 
    return QueryResult(std::move(ret_lines));
}

// 或查询
QueryResult OrQuery::eval(const TextQuery &text) const
{
    QueryResult right = rhs.eval(text), left= lhs.eval(text);
    set<LINENO> ret_lines;
    // 使用set_union求并集
    set_union(left.begin(), left.end(), 
                right.begin(), right.end(), 
                inserter(ret_lines, ret_lines.begin()));

    return QueryResult(std::move(ret_lines));
}

// 非查询
QueryResult NotQuery::eval(const TextQuery &text) const
{
    QueryResult result = query.eval(text);
    set<LINENO> ret_lines;
    int full_size = result.getSizeOfString();
    for (LINENO n = 1; n <= full_size; ++n) { // 遍历所有行
        if (result.hasLintNo(n) == false) { // 如果不存在
            ret_lines.insert(n); // 插入
        }
    }
    return QueryResult(std::move(ret_lines));
}
