// 语法解析
#pragma once
#include <regex>
using std::regex, std::smatch;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <iostream>
using std::cout, std::endl;

#include "colors.h"

struct Token {
    std::string type; // bracket: 括号, operator: 运算符, word: 单词, variable: 变量, number: 数字
    std::string value;
    Token(std::string type, std::string value) : type(type), value(value) {}
};

class Lexer {
private:
    std::string input;
    int pos = 0;
    std::regex number = std::regex(R"(\d+(\.\d+)?)"); // 匹配数字
    std::regex bracket = std::regex(R"([\(\)])"); // 匹配括号
    std::regex operatorr = std::regex(R"([=~&|])"); // 匹配运算符    
    std::regex variable = std::regex(R"([a-zA-Z_]\w*)"); // 匹配变量
    std::regex word = std::regex(R"((\"[^\"]*\")|\w+)"); // 匹配双引号包裹的内容或单词
    std::regex outputoperator = std::regex(R"(\s*>>\s*|\s*>\s*)");  // 重定向操作符
    std::regex filepath = std::regex(R"([a-zA-Z0-9_\-\.\/\\]+(\.[\w]+)?)"); // 文件路径
public:
    std::vector<Token> tokens; // 存储解析的tokens
    Lexer(std::string input) : input(input) { tokenize();  check(); checkbracket(); }
    Lexer() : input("") {}
    ~Lexer() {} 
    void tokenize(); // 解析字符串
    void showTokens(); // 打印tokens
    void check(); // 检查tokens
    void checkbracket(); // 检查括号    
};

void Lexer::tokenize() {
    std::smatch match;
    bool expectFilePath = false;  // 标记是否遇到 >>

    while (pos < input.length()) {
        std::string::const_iterator start = input.begin() + pos;
        std::string::const_iterator end = input.end();
        if (std::regex_search(start, end, match, outputoperator) && match.position() == 0) {
            expectFilePath = true;  // 遇到 >> 标记
            pos += match.length();
        }
        else if (expectFilePath) {
            // 将剩余所有内容作为文件路径
            std::string remainingText = input.substr(pos);
            // 去除首尾空格
            remainingText = std::regex_replace(remainingText, std::regex("^\\s+|\\s+$"), "");
            tokens.push_back(Token{"filepath", remainingText});
            pos = input.length(); // 直接移动到结尾
            expectFilePath = false;
        }
        else if (std::regex_search(start, end, match, number) && match.position() == 0) {
            tokens.push_back(Token{"number", match.str()});
            pos += match.length();
        } else if (std::regex_search(start, end, match, bracket) && match.position() == 0) {
            tokens.push_back(Token{"bracket", match.str()});
            pos += match.length();
        } else if (std::regex_search(start, end, match, operatorr) && match.position() == 0) {
            tokens.push_back(Token{"operator", match.str()});
            pos += match.length();
        } else if (std::regex_search(start, end, match, variable) && match.position() == 0) {
            tokens.push_back(Token{"variable", match.str()});
            pos += match.length();
        } else if (std::regex_search(start, end, match, word) && match.position() == 0) {
            // 去除双引号
            if (match.str()[0] == '\"') {
                tokens.push_back(Token{"word", match.str().substr(1, match.str().length() - 2)});
            } else {
                tokens.push_back(Token{"word", match.str()});
            }
            pos += match.length();
        } else if (std::regex_search(start, end, match, filepath) && match.position() == 0) {
            tokens.push_back(Token{"filepath", match.str()});
            pos += match.length();
        } 
        else {
            pos++;
        }
    }
}

void Lexer::showTokens() {
    for (auto token : tokens) {
        if (token.type == "number") {
            std::cout << GREEN << "number" << RESET << " " << token.value << std::endl;
        } else if (token.type == "bracket") {
            std::cout << YELLOW << "bracket" << RESET << " " << token.value << std::endl;
        } else if (token.type == "operator") {
            std::cout << BLUE << "operator" << RESET << " " << token.value << std::endl;
        } else if (token.type == "variable") {
            std::cout << MAGENTA << "variable" << RESET << " " << token.value << std::endl;
        } else if (token.type == "word") {
            std::cout << CYAN << "word" << RESET << " " << token.value << std::endl;
        }
        else if (token.type == "filepath") {
            std::cout << PINK << "filepath" << RESET << " " << token.value << std::endl;
        } 
        else {
            std::cout << RED << "error" << RESET << " " << token.value << std::endl;
        }
    }
}

void Lexer::check() {
    if (tokens.size() == 3 && tokens[1].type == "operator" && tokens[1].value == "=") {
        tokens[0].type = "variable";
        tokens[2].type = "word";
    } else if (tokens.size() == 1 && tokens[0].type == "variable") {
        tokens[0].type = "word";
    } else {
        for (int i = 2; i < tokens.size(); i++) {
            if (tokens[i].type == "operator" && tokens[i].value == "=") {
                throw std::runtime_error("Syntax error: '=' at position " + std::to_string(i));
            }
        }
    }
}
inline void Lexer::checkbracket()
{
    int left = 0, right = 0;
    for (auto token : tokens) {
        if (token.type == "bracket") {
            if (token.value == "(") {
                left++;
            } else {
                right++;
            }
        }
    }
    if (left != right) {
        throw std::runtime_error("Syntax error: brackets do not match");
    }
}
