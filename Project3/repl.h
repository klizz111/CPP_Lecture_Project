// 交互环境实现
#pragma once
#include "lexer.h"
#include "colors.h"
#include "file.h"
#include "query.h"
#include "formatter.h"  
#include <unordered_map>
using std::unordered_map;
#include <string>
using std::string;
#include <stack>
using std::stack;
#include <functional>

class Repl {
private:
    Lexer lexer;
    string input;
    unordered_map<string , string> variables;// 存储变量
    vector<fstream> files; // 文件
    vector<string> file_names_list; // 文件名列表
    bool redirect = false; // 是否重定向
    vector<string> outPutFileList; // 输出文件列表
public:
    Repl() = default;
    void mainloop(); // 主循环
    void interpret(); // 解释器
    string showrep(); // 显示查询语句
    vector<Token> inversedPolish(); // 中缀表达式转逆波兰表达式
    Query buildQueryFromRPN(const vector<Token> &rpnTokens); // 根据逆波兰表达式构造查询
    void checkifredirect();
    std::function<void(const Result&, int)> outputToScreen = [this](const Result &result, int i) {
        cout << "In file " << GREEN << this->file_names_list[i] << RESET << " : " << endl;
        cout << DEEP_GREEN << this->showrep() << RESET << " occurs " << result.getTimes() << " times." << endl;
        cout << result;
        cout << "--------------------------------" << endl;
    };
    std::function<void(const Result&, int)> outputToFile = [this](const Result &result, int i) {
        ofstream outPutFile(this->outPutFileList[0]);
        outPutFile << "In file " << this->file_names_list[i] << " : " << endl;
        outPutFile << this->showrep() << " occurs " << result.getTimes() << " times." << endl;
        result.outToFile(outPutFile);
        outPutFile << "--------------------------------" << endl;
        cout << GREEN << "Output to file " << this->outPutFileList[0] << RESET << endl;
    };
};

inline  void Repl::mainloop() {
    cin.get(); // 清空缓冲区
    try_again:
    // 清空文件列表
    files.clear();
    cout << BLUE << "请输入文件名, 以分号分隔: " << RESET << endl;
    cout << ">>> ";
    getline(cin, input);
    file_names_list = splitFilename(input);
    try {
        readfile(file_names_list, files);
    } catch (std::runtime_error &e) {
        cout << e.what() << endl;
        goto try_again;
    }
    while (1) {
        cout << BRIGHT_YELLOW << ">>> " << RESET;
        getline(cin, input); 
        if (input == "exit" || input == "quit" || input == "q") {
            break;
        } 
        try {
            lexer = Lexer(input);
            interpret();
        } catch (std::runtime_error &e) {
            cout << RED << e.what() << RESET << endl;
        }
    }
    return;
}

// 解释器
inline void Repl::interpret() {
    checkifredirect();
    // 情况一：单个单词查询
    if (lexer.tokens.size() == 1 && lexer.tokens[0].type == "word") {
        // 先尝试在变量中查找
        if (variables.find(lexer.tokens[0].value) != variables.end()) {
            cout << variables[lexer.tokens[0].value] << endl;
            return;
        }
        // 尝试在文件中查找
        int i = 0;
        for (auto &file : files) {
            file.clear();
            file.seekg(0);
            auto result = Formatter(0,&file).format();
            result = TextQuery(result).query(lexer.tokens[0].value);
            if (redirect) {
                outputToFile(result,i++);
            } else {
                outputToScreen(result,i++);
            }
            Result::clear();
        }
        return;
    }

    // 情况二: 赋值
    else if (lexer.tokens.size() == 3 && lexer.tokens[1].type == "operator" && lexer.tokens[1].value == "=") {
        if (lexer.tokens[0].type != "variable") {
            throw std::runtime_error("Syntax error: expected variable");
        }
        if (lexer.tokens[2].type != "word") {
            throw std::runtime_error("Syntax error: expected word");
        }
        variables[lexer.tokens[0].value] = lexer.tokens[2].value;
        cout << GREEN << "Variable " << lexer.tokens[0].value << " set to " << lexer.tokens[2].value << RESET << endl;
    }

    // 情况三: 复杂查询
    else {
        auto outputQueue = inversedPolish();
        Query finalQuery = buildQueryFromRPN(outputQueue);
        int i = 0;
        for (auto &file : files) {
            file.clear();
            file.seekg(0);
            auto formatted = Formatter(0, &file).format();
            auto result = finalQuery.eval(TextQuery(formatted));
            if (redirect) {
                outputToFile(result,i++);
            } else {
                outputToScreen(result,i++);
            }
            Result::clear();
        }
    }
}

// 显示查询语句
inline string Repl::showrep()
{
    string rep;
    for (auto &tk : lexer.tokens) {
        if (tk.type == "word") rep += "\"" + tk.value + "\"";
        else if (tk.type == "operator" && tk.value == "=") continue; 
        else rep += tk.value;
    }
    return rep;
}

inline vector<Token> Repl::inversedPolish() {
    // 将variable改为word
    for (auto &tk : lexer.tokens) {
        if (tk.type == "variable") {
            if (variables.find(tk.value) != variables.end()) {
                tk.type = "word";
                tk.value = variables[tk.value];
            } else {
                throw std::runtime_error("Variable " + tk.value + " not found");
            }
        }
    }

    vector<Token> outputQueue;
    stack<Token> operatorStack;

    // 优先级与左结合性
    auto precedence = [&](const string &op) {
        if (op == "~") return 3;
        if (op == "&") return 2;
        if (op == "|") return 1;
        return 0;
    };
    auto isLeftAssociative = [&](const string &) { 
        return true; 
    };

    // Shunting-Yard
    for (auto &tk : lexer.tokens) {
        if (tk.type == "word" || tk.type == "number") {
            outputQueue.push_back(tk);
        } else if (tk.type == "operator") {
            while (!operatorStack.empty() && operatorStack.top().type == "operator" &&
                    ((isLeftAssociative(tk.value) && precedence(tk.value) <= precedence(operatorStack.top().value)) ||
                    (!isLeftAssociative(tk.value) && precedence(tk.value) < precedence(operatorStack.top().value)))) {
                outputQueue.push_back(operatorStack.top());
                operatorStack.pop();
            }
            operatorStack.push(tk);
        } else if (tk.type == "bracket" && tk.value == "(") {
            operatorStack.push(tk);
        } else if (tk.type == "bracket" && tk.value == ")") {
            while (!operatorStack.empty() && !(operatorStack.top().type == "bracket" && operatorStack.top().value == "(")) {
                outputQueue.push_back(operatorStack.top());
                operatorStack.pop();
            }
            if (!operatorStack.empty()) {
                operatorStack.pop();
            } else {
                throw std::runtime_error("Mismatched parentheses");
            }
        }
    }

    while (!operatorStack.empty()) {
        if (operatorStack.top().type == "bracket") {
            throw std::runtime_error("Mismatched parentheses");
        }
        outputQueue.push_back(operatorStack.top());
        operatorStack.pop();
    }

    return outputQueue;
}

inline Query Repl::buildQueryFromRPN(const vector<Token> &rpnTokens) {
    std::stack<Query> st;
    for (auto &tk : rpnTokens) {
        // 忽略末尾无用的"="
        if (tk.type == "operator" && tk.value == "=") {
            continue;
        }
        if (tk.type == "word") {
            st.push(Query(tk.value));
        } else if (tk.type == "operator") {
            if (tk.value == "~") {
                Query topQ = st.top(); 
                st.pop();
                st.push(~topQ);
            } else if (tk.value == "&") {
                Query rhs = st.top(); 
                st.pop();
                Query lhs = st.top(); 
                st.pop();
                st.push(lhs & rhs);
            } else if (tk.value == "|") {
                Query rhs = st.top(); 
                st.pop();
                Query lhs = st.top(); 
                st.pop();
                st.push(lhs | rhs);
            }
        }
    }
    // 最终只应有一个查询对象
    if (st.size() != 1) {
        throw std::runtime_error("RPN parse error");
    }
    return st.top();
}

void Repl::checkifredirect() {
    // 判断最后一个token是否为filepath
    if (lexer.tokens.back().type == "filepath") {
        redirect = true;
        outPutFileList = splitFilename(lexer.tokens.back().value);
        lexer.tokens.pop_back();
    }
}
