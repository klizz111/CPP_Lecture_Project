// 文件读取
#pragma once
#include <fstream>
using std::fstream;

#include <iostream>
using std::cout, std::endl, std::cin;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <exception>
using std::runtime_error;

#include <ios>
#include <regex>
using std::regex, std::smatch;

#include <string>   
using std::string;

#include "colors.h"

// 前向声明
vector<string> splitFilename(const string str);

// 读取文件,支持多个文件
void readfile(const string &filename, vector<fstream> &files) {
    fstream file;
    vector<string> filesnames = splitFilename(filename);
    for (auto &name : filesnames) {
        file.open(name, fstream::in);
        if (!file.is_open()) {
            throw runtime_error(string(REABACKGROUND) + "Open file " + name + " failed." + string(BACKGROUNDRESET));
        }
        files.push_back(move(file));
    }
    if (files.size() == 0) {
        throw runtime_error(string(REABACKGROUND) + "No file opened." + string(BACKGROUNDRESET));
    }
    cout << GREEN << "Open " << files.size() << " files." << endl;
    cout << "Read files successfully." << RESET << endl;
}
void readfile(const vector<string> &filenames, vector<fstream> &files) {
    fstream file;
    for (auto &name : filenames) {
        file.open(name, fstream::in);
        if (!file.is_open()) {
            throw runtime_error(string(REABACKGROUND) + "Open file " + name + " failed." + string(BACKGROUNDRESET));
        }
        files.push_back(move(file));
    }
    if (files.size() == 0) {
        throw runtime_error(string(REABACKGROUND) + "No file opened." + string(BACKGROUNDRESET));
    }
    cout << GREEN << "Open " << files.size() << " files." << endl;
    cout << "Read files successfully." << RESET << endl;
}

// 处理文件路径输入
vector<string> splitFilename(const string str) {
    regex pattern(R"(([^;]+\.txt))");
    int pos = 0;
    smatch result;
    vector<string> filesnames;
    while (pos < str.length()) {
        string::const_iterator start = str.begin() + pos;
        string::const_iterator end = str.end();
        if (regex_search(start, end, result, pattern) && result.position() == 0) {
            filesnames.push_back(result.str());
            pos += result.length();
        } else {
            pos++;
        }
    }
    return filesnames;
}