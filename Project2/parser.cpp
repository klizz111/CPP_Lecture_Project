#include "parser.h"
#include <stack>

void Parser::mainloop()
{
    while(true) {
        cout << ">>> ";
        getline(cin, input);
        checkexit();
        try {
            //showVariables();
            ast = AST(input); // 重新初始化AST对象
            root = nullptr; // 重置root指针
            //ast.printLex();
            ast.buildAST();
            ast.checkAST(ast.root);
            root = ast.root; 
            checkvariablename();// 检查变量名是否合法
            //ast.printAST(ast.root, 0);
            parse();
        } catch (const std::runtime_error &e) {
            cout << e.what() << endl;
            continue;
        }
        
    }
}


void Parser::checkexit()
{
    if (input == "exit" || input == "quit" || input == "q") {
        cout << "Using 'exit()' or 'quit()' to exit the program" << endl;
    } else if (input == "exit()" || input == "quit()" || input == "q()") {
        cout << "Exiting..." << endl;
        exit(0);
    }
}

void Parser::checkvariablename()
{
    // 检查变量名是否合法
    // 遍历所有的token，如果变量名不合法，抛出异常
    for (auto token : ast.tokens) {
        if (token.type == "VARIABLE" && find(__builtin__type.begin(), __builtin__type.end(), token.value) != __builtin__type.end()) {
            throw runtime_error("Error: Invalid variable name " + token.value);
        }
    }
}

complexObject Parser::parseExpression() {
    try{
        ckeckVariableExist();
    } catch (const std::runtime_error &e) {
        cout << e.what() << endl;
        return complexObject();
    }
    return evaluateNode(root);
}

complexObject Parser::evaluateNode(ASTNode* node) {
    if (node->type == "OPERATOR") {
        complexObject left = evaluateNode(node->children[0]);
        complexObject right = evaluateNode(node->children[1]);
        if (node->value == "+") {
            return left + right;
        } else if (node->value == "-") {
            return left - right;
        } else if (node->value == "*") {
            return left * right;
        } else if (node->value == "/") {
            return left / right;
        } else if (node->value == "^") {
            return left ^ right;
        } else {
            throw runtime_error("Error: Unknown operator " + node->value);
        }
    } else if (node->type == "NUMBER") {
        return complexObject(node->value);
    } else if (node->type == "VARIABLE") {
        if (variablesList.find(node->value) == variablesList.end()) {
            throw runtime_error("Error: Variable " + node->value + " does not exist");
        }
        return variablesList[node->value];
    } else if (node->type == "FUNCTION") {
        return parseFunction(node);
    } else if (node->type == "EXPRESSION") {
        return evaluateNode(node->children[0]);
    } else {
        throw runtime_error("Error: Unknown node type " + node->type);
    }
}


void Parser::ckeckVariableExist()
{
    // 检查变量是否存在
    // 遍历所有的变量，如果变量不存在，抛出异常
    for (auto token : ast.tokens) {
        if (token.type == "VARIABLE" && variablesList.find(token.value) == variablesList.end()) {
            throw runtime_error("Error: Variable " + token.value + " does not exist");
        }
    }
}

void Parser::parse()
{
    // 解析ASSIGMENT类型
    if (root->type == "ASSIGNMENT" && root->value == "=") {
        string varname = root->children[0]->value;
        complexObject value = evaluateNode(root->children[1]);
        variablesList[varname] = value;
    } 
    else if (root->type == "ASSIGNMENT" && (root->value == "++" || root->value == "--")) {
        string varname = root->children[0]->value;
        if (variablesList.find(varname) == variablesList.end()) {
            throw runtime_error("Error: Variable " + varname + " does not exist");
        }
        if (root->value == "++") {
            complexObject value = variablesList[varname];
            value++;
            variablesList[varname] = value;
            return;
        } else {
            complexObject value = variablesList[varname];
            value--;
            variablesList[varname] = value;
            return;
        }
    }
    else if (root->type == "ASSIGNMENT" && (root->value == "+=" )) {
        variablesList[root->children[0]->value] += evaluateNode(root->children[1]);
    }
    else if (root->type == "ASSIGNMENT" && (root->value == "-=" )) {
        variablesList[root->children[0]->value] -= evaluateNode(root->children[1]);
    }
    else if (root->type == "ASSIGNMENT" && (root->value == "*=" )) {
        variablesList[root->children[0]->value] *= evaluateNode(root->children[1]);
    }
    else if (root->type == "ASSIGNMENT" && (root->value == "/=" )) {
        variablesList[root->children[0]->value] /= evaluateNode(root->children[1]);
    }
    // 解析EXPRESSION类型
    else if (root->type == "EXPRESSION") {
        complexObject res = parseExpression();
        cout << "ans = " << res.getRes() << endl;
    }
}

void Parser::showVariables()
{
    for (auto var : variablesList) {
        cout << var.first << " = " << var.second.getRes() << endl;
    }
}

void Parser::parseAssignment() {
    if (root->children.size() != 2) {
        throw runtime_error("Error: Invalid assignment");
    }
    string varName = root->children[0]->value;
    complexObject value = evaluateNode(root->children[1]);
    if (root->value == "+=") {
        variablesList[varName] += value;
    } else if (root->value == "-=") {
        variablesList[varName] -= value;
    } else {
        variablesList[varName] = value;
    }
}

complexObject Parser::parseTerm()
{
    return complexObject();
}

complexObject Parser::parseFactor()
{
    return complexObject();
}

complexObject Parser::parseFunction(ASTNode* node) {
    // 解析函数调用
    if (node->value == "con") {
        if (node->children.size() != 1) {
            throw runtime_error("Error: Function 'con' expects one argument");
        }
        complexObject arg = evaluateNode(node->children[0]);
        return con(arg); 
    } else if (node->value == "mod") {
        if (node->children.size() != 1) {
            throw runtime_error("Error: Function 'mod' expects one argument");
        }
        complexObject arg = evaluateNode(node->children[0]);
        return mod(arg); 
    } else {
        throw runtime_error("Error: Unknown function " + node->value);
    }
}

complexObject Parser::parseValue()
{
    return complexObject();
}
