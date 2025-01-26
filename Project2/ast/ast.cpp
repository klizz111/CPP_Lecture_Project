#include "ast.h"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <regex>
#include <string>
#include <algorithm>

using namespace std;

vector<Token> Lexer::tokenize() {
    vector<Token> tokens;
    smatch match;

    while (pos < text.length()) {
        string::const_iterator start = text.begin() + pos;
        string::const_iterator end = text.end();

        if (regex_search(start, end, match, number) && match.position() == 0) {
            tokens.push_back(Token("NUMBER", match.str()));
            pos += match.length();
        } else if (regex_search(start, end, match, variable) && match.position() == 0) {
            if (match.str() == "i") {
                tokens.push_back(Token("NUMBER", match.str()));
            } else {
                if (find(__builtin__func__.begin(), __builtin__func__.end(), match.str()) != __builtin__func__.end()) {
                    tokens.push_back(Token("FUNCTION", match.str()));
                } else {
                    tokens.push_back(Token("VARIABLE", match.str()));
                }
            }
            pos += match.length();
        } else if (regex_search(start, end, match, operatorr) && match.position() == 0) {
            tokens.push_back(Token("OPERATOR", match.str()));
            pos += match.length();
        } else if (regex_search(start, end, match, brakets) && match.position() == 0) {
            tokens.push_back(Token("BRACKET", match.str()));
            pos += match.length();
        } else {
            pos++;
        }
    }
    return tokens;
}

void AST::buildAST()
{
    root = new ASTNode();
    pos = 0;
    if (tokens.size() == 0) return;

    ///
    if (tokens.size()==1 && tokens[0].type == "VARIABLE")
    {
        root->type = "EXPRESSION";
        ASTNode* variableNode = new ASTNode();
        variableNode->type = "VARIABLE";
        variableNode->value = tokens[0].value;
        root->children.push_back(variableNode);
        return;
    }
    ///
    
    if (tokens[0].type == "VARIABLE")
    {
        if (tokens.size() > 1 && (tokens[1].value == "=" || tokens[1].value == "+=" || tokens[1].value == "-=" 
            || tokens[1].value == "*=" || tokens[1].value == "/=" ))
        {
            if (tokens.size() < 3) {
                throw runtime_error("Error: Missing value for assignment at token " + to_string(pos) + " with value " + tokens[pos].value);
            }
            root->type = "ASSIGNMENT";
            root->value = tokens[1].value; // 赋值操作符
            ASTNode* variableNode = new ASTNode();
            variableNode->type = "VARIABLE";
            variableNode->value = tokens[0].value;
            root->children.push_back(variableNode);
            pos = 2; // 跳过变量和赋值操作符
            root->children.push_back(parseExpression());
        }
        else if (tokens[1].value == "++" || tokens[1].value == "--")
        {
            root->type = "ASSIGNMENT";
            root->value = tokens[1].value; // 赋值操作符
            ASTNode* variableNode = new ASTNode();
            variableNode->type = "VARIABLE";
            variableNode->value = tokens[0].value;
            root->children.push_back(variableNode);
        }
        else
        {
            root->type = "EXPRESSION";
            root->children.push_back(parseExpression());
        }
    }
    else if (tokens[0].type == "FUNCTION")
    {
        root->type = "EXPRESSION";
        root->children.push_back(parseFunction());
    }
    else
    {
        root->type = "EXPRESSION";
        root->children.push_back(parseExpression());
    }
}

ASTNode* AST::parseExpression() {
    ASTNode* node = parseTerm();
    while (pos < tokens.size() && (tokens[pos].value == "+" || tokens[pos].value == "-")) {
        ASTNode* newNode = new ASTNode();
        newNode->type = "OPERATOR";
        newNode->value = tokens[pos].value;
        pos++;
        newNode->children.push_back(node);
        newNode->children.push_back(parseTerm());
        node = newNode;
    }
    return node;
}

ASTNode* AST::parseTerm() {
    ASTNode* node = parseFactor();
    while (pos < tokens.size() && (tokens[pos].value == "*" || tokens[pos].value == "/" || tokens[pos].value == "^")) {
        ASTNode* newNode = new ASTNode();
        newNode->type = "OPERATOR";
        newNode->value = tokens[pos].value;
        pos++;
        newNode->children.push_back(node);
        newNode->children.push_back(parseFactor());
        node = newNode;
    }
    return node;
}

ASTNode* AST::parseFactor() {
    if (tokens[pos].value == "(") {
        pos++;
        ASTNode* node = parseExpression();
        if (pos >= tokens.size() || tokens[pos].value != ")") {
            throw runtime_error("Error: Missing closing bracket at token " + to_string(pos) + " with value " + tokens[pos].value);
        }
        pos++;
        return node;
    } else if (tokens[pos].type == "FUNCTION") {
        return parseFunction();
    } else {
        return parseValue();
    }
}

ASTNode* AST::parseValue() {
    if (tokens[pos].type == "NUMBER" || tokens[pos].type == "VARIABLE") {
        ASTNode* node = new ASTNode();
        node->type = tokens[pos].type;
        node->value = tokens[pos].value;
        pos++;
        return node;
    } else {
        throw runtime_error("Error: Unexpected token at position " + to_string(pos) + " with value " + tokens[pos].value);
    }
}

ASTNode* AST::parseFunction() {
    ASTNode* node = new ASTNode();
    node->type = "FUNCTION";
    node->value = tokens[pos].value;
    pos++;
    if (pos >= tokens.size() || tokens[pos].value != "(") {
        throw runtime_error("Error: Missing opening bracket for function at token " + to_string(pos) + " with value " + tokens[pos].value);
    }
    pos++;
    if (pos < tokens.size() && tokens[pos].value == ")") {
        throw runtime_error("Error: Function has no parameters at token " + to_string(pos) + " with value " + tokens[pos].value);
    }
    while (pos < tokens.size() && tokens[pos].value != ")") {
        node->children.push_back(parseExpression());
        if (pos < tokens.size() && tokens[pos].value == ",") {
            pos++;
        }
    }
    if (pos >= tokens.size() || tokens[pos].value != ")") {
        throw runtime_error("Error: Missing closing bracket for function at token " + to_string(pos) + " with value " + tokens[pos].value);
    }
    pos++;
    return node;
}

void AST::printAST(ASTNode* node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; ++i) cout << "-";
    cout << " " << node->type << " " << node->value << endl;
    for (auto child : node->children) {
        printAST(child, depth + 1);
    }
}

void AST::printLex()
{
    for (auto token : tokens) {
        cout << token.type << " " << token.value << endl;
    }
}

void AST::checkAST(ASTNode* node) {
    if (!node) return;

    int astNumberCount = 0;
    int astVariableCount = 0;

    std::function<void(ASTNode*)> countASTNodes = [&](ASTNode* node) {
        if (node->type == "NUMBER") {
            ++astNumberCount;
        } else if (node->type == "VARIABLE") {
            ++astVariableCount;
        }
        for (auto child : node->children) {
            countASTNodes(child);
        }
    };

    countASTNodes(node);

    int tokenNumberCount = 0;
    int tokenVariableCount = 0;

    for (auto token : tokens) {
        if (token.type == "NUMBER") {
            ++tokenNumberCount;
        } else if (token.type == "VARIABLE") {
            ++tokenVariableCount;
        }
    }

    if (astNumberCount != tokenNumberCount || astVariableCount != tokenVariableCount) {
        throw std::runtime_error("SyntaxError: invalid syntax");
    }
}