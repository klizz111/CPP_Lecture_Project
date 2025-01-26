#include "ast/ast.h"
#include "complexobject/complexObject.h"
#include <unordered_map>
#include <regex>

class Parser {
public:
    Parser() {};
    void mainloop();
    string input;
    unordered_map<string, complexObject> variablesList;
    vector<string> __builtin__type = {"i", "e", "con", "mod"};
    void checkexit();
    void checkvariablename();
    void parseAssignment();
    complexObject parseTerm();
    complexObject parseExpression();
    complexObject parseFactor();
    complexObject parseFunction(ASTNode* node);
    complexObject parseValue();
    complexObject evaluateNode(ASTNode* node);
    void ckeckVariableExist();
    void parse();
    void showVariables();
    AST ast;
    size_t pos;
    ASTNode* &root = ast.root;
};