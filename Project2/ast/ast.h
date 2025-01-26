#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <stdexcept>
#include "../complexobject/complexObject.h"

using namespace std;

// 词法分析器生成的Token
class Token {
public:
    string type;
    string value;

    Token(string type, string value) : type(type), value(value) {}
};

// 词法分析器
class Lexer {
private:
    string text;
    size_t pos;
    const regex number = regex(R"(\d+(\.\d+)?i?)");// 匹配数字或复数
    const regex variable = regex(R"([a-zA-Z_]\w*)");// 匹配变量
    const regex operatorr = regex(R"(\+\+|--|\+=|-=|\*=|/=|\^|[\+\-\*/=])"); // 匹配操作符，包括 ++, --, +=, -=, *=, /=, ^
    const regex brakets = regex(R"([\(\)])");// 匹配括号

public:
    Lexer(string text) : text(text), pos(0) {}
    vector<string> __builtin__func__ = {"con", "mod"}; // 内置函数
    vector<string> __builtin__type = {"i", "e"}; // 内置类型
    vector<Token> tokenize();
};

// 构建AST
struct ASTNode {
    string type;// 节点类型
    string value;// 节点值
    vector<ASTNode*> children;
};
class AST : public Lexer {
private:
    size_t pos;
public:
    ASTNode* root;
    vector<Token> tokens;
    AST(string strs) : Lexer(strs), pos(0) {
        tokens = tokenize();
    }
    AST() : Lexer(""), pos(0) {}
    AST& operator=(const AST& ast) {
        if (this == &ast) {
            return *this;
        }
        root = ast.root;
        tokens = ast.tokens;
        pos = 0;
        return *this;
    }
    void buildAST();// 构建AST
    ASTNode* parseExpression();
    ASTNode* parseTerm();
    ASTNode* parseFactor();
    ASTNode* parseValue();
    ASTNode* parseFunction();
    void printAST(ASTNode* node, int depth);
    void printLex();
    void checkAST(ASTNode* node);
};