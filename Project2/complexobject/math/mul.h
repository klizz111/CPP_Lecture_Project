#ifndef MUL_H
#define MUL_H
#include <iostream>
#include <string>
#include <chrono>
#include <cmath>
#include <complex>
#include <vector>

class Mul{ 
public:
    Mul(const std::string &n1,const std::string &n2,const std::vector<std::string> &options = {});
    ~Mul();    
    std::string retur_res() { return result; }
private:
    std::string num1;
    std::string num2;
    std::string result;
    std::chrono::high_resolution_clock::time_point start_time; 
    std::chrono::high_resolution_clock::time_point end_time;
    std::chrono::duration<double> duration;
    std::string MU(const std::string &n1,const std::string &n2);
    bool show_time = false, scientific_notation = false, precision = false, usekaratsuba = false;
    int precision_num(std::string &s);
    
    //KaratSuba算法
    inline std::string addStrings(const std::string &num1, const std::string &num2);
    inline std::string subtractStrings(const std::string &num1, const std::string &num2);
    inline std::string karatsuba(const std::string &num1, const std::string &num2);
};

//mul.h
//高精度加法
std::string addStrings(const std::string &num1, const std::string &num2);

//高精度减法
std::string subtractStrings(const std::string &num1, const std::string &num2);

std::string stringMultiple(const std::string &num1, const std::string &num2);
std::string stringDivide(const std::string &num1, const std::string &num2);
std::string stringPow(const std::string &num1, const std::string &num2);
std::string stringSqrt(const std::string &num1, const std::string &num2);
#endif