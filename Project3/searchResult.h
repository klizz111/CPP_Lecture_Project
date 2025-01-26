// 存储搜索结果
#pragma once
#include <iostream>
using std::ostream, std::ofstream, std::cout, std::endl; 
#include <string>
using std::string;
#include <vector>
using std::vector;  
#include <memory>
using std::shared_ptr, std::make_shared;
#include <set>
using std::set;
#include <algorithm>
using std::set_intersection, std::iterator;
#include <fstream>  
using std::fstream;

#include "colors.h"

#define LINENO int // 行号

class Formatter; // 前置声明
// 存储格式化结果与搜索结果
class Result {
protected:
    set<LINENO> nos; // 出现的行号,使用set去重
    static vector<string> input; // 输入的内容,使用stattic避免重复拷贝
    int times = 0; // 出现次数  
public:
    Result() = default;
    friend ostream& operator<<(ostream &os, const Result &qr); // 输出结果到屏幕
    friend ofstream& operator<<(ofstream &os, const Result &qr); // 输出结果到文件
    friend Formatter;
    static void clear() {input.clear();} // 清空结果 
    Result& operator=(Result &&qr) noexcept; // 移动赋值
    Result(const Result &qr); // 拷贝构造函数
    Result(set<LINENO> &&nos) : nos(std::move(nos)) {times = this->nos.size();} // 移动构造函数
    friend class TextQuery; // 声明为友元类
    int getTimes() const {return times;} // 获取出现次数
    set<LINENO>::iterator begin() {return nos.begin();} // 获取nos的起始迭代器
    set<LINENO>::iterator end() {return nos.end();} // 获取nos的结束迭代器
    int getSizeOfString() const {return input.size();} // 获取总行数
    bool hasLintNo(LINENO no) {return nos.find(no) != nos.end();} // 判断是否存在行号no
    void showcurrentinput() { // 输出当前输入
        for (auto &i : input) {
            cout << i << endl;
        }
    }
    static void setInput(vector<string> &input) { // 设置输入
        Result::input = input;
    }
    void outToFile(ofstream &os) const {
        for (auto i : nos) {
            if (input[i-1][0] == ' ') os << "(line " << i << ") " << input[i-1].substr(1) << endl; // 去除空格
            else os << "(line " << i << ") " << input[i-1] << endl;
        }
    }
};

