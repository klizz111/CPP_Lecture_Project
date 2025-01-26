#ifndef QUERY_H
#define QUERY_H
#include <iostream>
#include <string>
#include <memory>
#include "TextQuery.h"

using QueryResult = Result;

class Query_base {
    friend class Query;
protected:
    virtual ~Query_base() = default;

private:
    virtual QueryResult eval(const TextQuery&) const = 0;
/*     virtual std::string rep() const = 0; // 存储查询指令 */
};

class WordQuery : public Query_base {
    // Query使用WordQuery的构造函数
    friend class Query;
    WordQuery(const std::string& s) : query_word(s) {}

    QueryResult eval(const TextQuery& t) const override {   
        return t.query(query_word); 
    }
    std::string query_word;
};

class Query {
private:
    Query(std::shared_ptr<Query_base> query) : q(query) {}
    std::shared_ptr<Query_base> q;
public:
    Query(const std::string& s) : q(new WordQuery(s)) {}
    QueryResult eval(const TextQuery& t) const { 
        return q->eval(t); 
    }

    friend Query operator~(const Query&);
    friend Query operator|(const Query&, const Query&);
    friend Query operator&(const Query&, const Query&);
};

class BinaryQuery : public Query_base {
protected:
    Query lhs, rhs; // 左右查询表达式
    std::string opSym; // 操作符
    BinaryQuery(const Query&l, const Query& r, std::string s) : lhs(l), rhs(r), opSym(s) {}
};

class OrQuery :public BinaryQuery {
    friend Query operator|(const Query&, const Query&);
    OrQuery(const Query& left, const Query& right) : BinaryQuery(left, right, "|") { }
    QueryResult eval(const TextQuery& )const override;
};

inline Query operator|(const Query &lhs, const Query& rhs) {
    return std::shared_ptr<Query_base>(new OrQuery(lhs, rhs));
}

class AndQuery : public BinaryQuery {
    friend Query operator&(const Query&, const Query&);
    AndQuery(const Query& left, const Query& right) : 
    BinaryQuery(left, right, "&") {}
    QueryResult eval(const TextQuery &) const override;
};

inline Query operator& (const Query& lhs, const Query& rhs) {
    return std::shared_ptr<Query_base>(new AndQuery(lhs, rhs));
}

class NotQuery : public Query_base {
    friend Query operator~(const Query& operand);
    NotQuery(const Query& q): query(q) {}
    QueryResult eval(const TextQuery &) const override; 
    Query query;
};

inline Query operator~(const Query& operand) {
    return std::shared_ptr<Query_base>(new NotQuery(operand));
}

#endif