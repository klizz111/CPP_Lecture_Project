// searchResult.cpp
#include "searchResult.h"

vector<string> Result::input; // 初始化静态成员变量

ostream& operator<<(ostream &os, const Result &qr) { // 输出结果到屏幕
    for (auto i : qr.nos) {
        if (Result::input[i-1][0] == ' ') os << BOLD << ITALIC << "(line " << i << ") " << RESET << Result::input[i-1].substr(1) << endl; // 去除空格
        else os << BOLD << ITALIC << "(line " << i << ") " << RESET << Result::input[i-1] << endl;
    }
    return os;
}

ofstream& operator<<(ofstream &os, const Result &qr) { // 输出结果到文件
    for (auto i : qr.nos) {
        if (Result::input[i-1][0] == ' ') os << Result::input[i-1].substr(1) << endl;
        else os << Result::input[i-1] << endl;
    }
    return os;
}

Result& Result::operator=(Result &&qr) noexcept { // 移动赋值
    if (this != &qr) {
        nos = std::move(qr.nos);
        times = qr.times;
    }
    return *this;
}

Result::Result(const Result &qr) { // 拷贝构造函数
    nos = qr.nos;
    times = qr.times;
}