#pragma once
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <memory>
using std::shared_ptr, std::make_shared, std::unique_ptr;
#include <iostream>
using std::ostream, std::ofstream, std::cout, std::cin, std::endl;
#include <fstream>
using std::fstream;
#include <map>
using std::map;

#include "searchResult.h"

// 格式化文件
class Formatter {
private:
    int lineMax; // 每行最大字符数
    fstream *files; // 文件, 使用指针, 避免发生拷贝
public:
    Formatter() = default;
    Formatter(int lineMax = 0, fstream *files = nullptr) : lineMax(lineMax), files(files) {
        if (lineMax < 0) {
            throw std::runtime_error("最大行数不能小于0");
        }
    }
    // 格式化文件
    Result format();
};

Result Formatter::format() {
    Result res;
    string line;
    int lineNo = 1;
    vector<string> tempInput; // 临时存储格式化结果
    
    // lineMax = 0, 不进行格式化
    if (lineMax == 0) {
        while (getline(*files, line)) {
            tempInput.push_back(line);
            res.nos.insert(lineNo++);
        }
        Result::setInput(tempInput); // 使用静态方法设置input
        return res;
    }
    
    // 进行格式化
    while (getline(*files, line)) {
        if (line.size() <= lineMax) {
            tempInput.push_back(line);
            res.nos.insert(lineNo++);
            continue;
        }
        
        // 一行超过lineMax
        int i = 0;
        while (i < line.size()) {
            int end = i + lineMax;
            if (end >= line.size()) {
                tempInput.push_back(line.substr(i));
                res.nos.insert(lineNo++);
                break;
            }
            // 找到最后一个空格或标点符号
            int j = end;
            while (j > i && line[j] != ' ' && line[j] != ',' && line[j] != '.' && line[j] != ';' && line[j] != ':') --j;
            if (j == i) {
                tempInput.push_back(line.substr(i, lineMax));
                res.nos.insert(lineNo++);
                i += lineMax;
            } else {
                tempInput.push_back(line.substr(i, j - i));
                res.nos.insert(lineNo++);
                i = j + 1;
            }
        }
    }
    Result::setInput(tempInput); // 使用静态方法设置input
    return res;
}