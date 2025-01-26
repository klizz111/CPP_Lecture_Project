// 实现主程序的杂项函数
#include <iostream>
using std::cout, std::cin, std::endl;
#include "colors.h"

// 显示主菜单
void showmenu() {
    cout << YELLOW << "选择需要的功能: " << RESET << endl;
    cout << BOLD;
    cout << "1. 文件格式化" << endl;
    cout << "2. 文本查询" << endl;
    cout << REABACKGROUND << "3. 退出" << BACKGROUNDRESET << endl;
    cout << RESET;
    cout << GREEN << SPARK <<  "请输入选择: " << RESET;
}

