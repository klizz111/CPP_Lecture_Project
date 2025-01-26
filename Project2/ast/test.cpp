#include "ast.h"

int main() {
    using namespace std;
    while(true){
    string text;    
    cout << "Enter the expression: ";
    getline(cin, text);    
    try {
        AST ast(text);
        //ast.printLex();
        ast.buildAST();
        ast.checkAST(ast.root);
        ast.printAST(ast.root, 0);
        //cout << endl;
    } catch (const std::runtime_error &e) {
        cout << e.what() << endl;
    }
    }
}