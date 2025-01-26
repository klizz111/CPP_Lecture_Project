#include "file.h"
#include "colors.h"
#include "misc.h"
#include "formatter.h"
#include "TextQuery.h"
#include "query.h"
#include "repl.h"

using namespace std;

int main() {
    int choice;
    vector<fstream> files;
    string input;
    vector<string> filesnames;
    vector<ofstream> out_put_file; 
    while (1) {
        files.clear(); // 清空文件
        showmenu();
        if (!(cin >> choice)) {
            cin.clear(); // 清空错误标志
            cin.ignore(1024, '\n'); // 忽略错误字符
            cout << RED << "请输入正确选项" << RESET << endl;
            continue;
        }
        switch (choice) {
            case 1: {// 文件格式化
                cin.clear();
                cout << BLUE << "请输入文件名, 以分号分隔: " << RESET << endl;
                cout << ">>> ";
                cin >> input;
                filesnames = splitFilename(input);
                try {
                    readfile(filesnames, files);
                } catch (runtime_error &e) {
                    cout << e.what() << endl;
                    break;
                }
                cout << BLUE << "请输入需要保存到的文件路径" << RESET << endl;
                cout << ">>> ";
                cin >> input;

                // 查询文件是否已经存在
                if (ifstream(input)) {
                    cout << RED << "文件已存在, 是否覆盖?(y/n)" << RESET << endl;
                    cout << ">>> ";
                    char c;
                    cin >> c;
                    if (c != 'y') {
                        cout << RED << "取消操作" << RESET << endl;
                        break;
                    }
                }

                cout << BLUE << "请输入每行最大字符数(0为不变)" << RESET << endl;
                cout << ">>> ";
                cin >> choice;

                out_put_file.clear();   
                out_put_file.push_back(ofstream(input)); // 使用 ofstream 打开文件

                for (auto &file : files) {
                    Formatter formatter(choice, &file);
                    Result res = formatter.format();
                    out_put_file[0] << res; // 使用 ofstream 的重载运算符
                    Result::clear(); // 清空结果
                }

                cout << GREEN << "格式化完成, 文件已保存" << RESET << endl;
                break;
            }

            case 2: {// 文本查询
                cin.clear();
                Repl().mainloop();
                break;
            }
            case 3:
                return 0;
            default:
                cout << RED << "请输入正确选项" << RESET << endl;
                break;
        }
        cout << endl;
    }
}