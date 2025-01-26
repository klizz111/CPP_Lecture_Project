#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <regex>
#include "mul.h"

Mul::Mul(const std::string &n1, const std::string &n2, const std::vector<std::string> &options){
    //检查输入是否为整型、浮点型或科学计数法
    auto is_valid_number = [](const std::string& s) {
        std::regex number_pattern(R"(^[-+]?\d*\.?\d+([eE][-+]?\d+)?$)");
        return std::regex_match(s, number_pattern);
    };
    if (!is_valid_number(n1) || !is_valid_number(n2)){
        throw "Error: input is not a number";
    }

    int pre;//精度

    //检查参数输入
    for (int i = 0; i < options.size(); i++){
        if (options[i] == "-d"){
            show_time = true;
        } else if (options[i].substr(0,2) == "-p"){
            try{
                precision = true;
                pre = std::stoi(options[i].substr(2));
                if (pre < 0){
                    throw "Error: invalid precision";
                }
            } catch (std::invalid_argument){
                throw "Error: invalid precision";
            }         
        } else if (options[i] == "-e"){
            scientific_notation = true;
        } else if (options[i] == "-k"){
            usekaratsuba = true;
        }
        else {
            throw "Error: invalid option";
        }
    }
    
    num1 = n1;
    num2 = n2;
    size_t maxlength;
    std::cout << num1 << " * " << num2 << " = ";

    //处理小数点与科学计数法
    int oct1 = precision_num(num1); 
    int oct2 = precision_num(num2);
    int oct = oct1 + oct2;//储存十的次方

    //正负数处理与判断
    auto is_negative_number = [](std::string& s) {
        if (s[0] == '-') {
            s = s.substr(1);
            return true;
        } else if (s[0] == '+') {
            s = s.substr(1);
            return false;
        } else {
            return false;
        }
    };
    bool is_negative = is_negative_number(num1) ^ is_negative_number(num2);

    //算法选择
    //计时开始
    start_time = std::chrono::high_resolution_clock::now();
    if (usekaratsuba) { 
        result = karatsuba(num1, num2);
    } else {
        result = MU(num1, num2);
    }
    //计时结束
    end_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);


    //补零
    if (oct > 0) {
        result = result + std::string(oct, '0');
    } else if (oct < 0) {
        if (result.length() < -oct) {
            result = std::string(-oct - result.length(), '0') + result;
        }
    }

    //精度处理
    if(precision){
        if (oct < 0){
            int int_length = result.length() + oct;
            result = result.substr(0, int_length + pre);
            result = result.insert(int_length, ".");
            if (result[0] == '.') {
                result = "0" + result;
            } else {
            }
        }
    }else{
        if (oct >= 0) {
        } else {
            result = result.insert(result.length() + oct, ".");
        }
        if (result[0] == '.') {
            result = "0" + result;
        }
    }

    //科学计数法
    if (scientific_notation) {
        //精度处理
        if (precision & oct >= 0) {
            auto s1 = result[0];
            auto s2 = result.substr(1, pre);
            auto l  = result.length() - 1;
            result = s1 + std::string(".") + s2 + "e" + std::to_string(l);
        } else {
            size_t dot_pos = result.find('.');
            if (dot_pos != std::string::npos) {
                auto left = result.substr(1, dot_pos).length();
                auto right = result.substr(dot_pos + 1).length();
                int e = left - right;
                result = result.erase(dot_pos, 1);
                result = result.insert(1, ".");
                result = result + "e" + std::to_string(e);
            } else {
                auto e = result.length() - 1;
                result = result.substr(0, 1) + "." + result.substr(1) + "e" + std::to_string(e);
            }
        }
    }

    if(is_negative){
        result = "-" + result;
    }
    std::cout << result << std::endl;
}

Mul::~Mul(){
    if (show_time){
        std::cout << "Time: " << duration.count() << " microseconds" << std::endl;
    }
}

//高精度乘法
std::string Mul::MU(const std::string &n1, const std::string &n2){

    //高精度乘法
    int len1 = n1.size();
    int len2 = n2.size();
    if (len1 == 0 || len2 == 0) return "0";

    std::vector<int> result(len1 + len2, 0);

    int i_n1 = 0;
    int i_n2 = 0;

    //从尾到头遍历第一个数的每一位
    for (int i = len1 - 1; i >= 0; i--) {
        int carry = 0;//进位
        int n1_digit = n1[i] - '0';

        i_n2 = 0;

        //从尾到头遍历第二个数的每一位
        for (int j = len2 - 1; j >= 0; j--) {
            int n2_digit = n2[j] - '0';
            /*模仿竖式相乘，n1_digit乘以n2_digit的每一位，carry存储进位数据，在下一轮循环中加入*/
            int sum = n1_digit * n2_digit + result[i_n1 + i_n2] + carry;
            carry = sum / 10;
            result[i_n1 + i_n2] = sum % 10;
            i_n2++;
        }

        if (carry > 0) {
            result[i_n1 + i_n2] += carry;
        }
        i_n1++;
    }

    int i = result.size() - 1;
    while (i >= 0 && result[i] == 0) i--;
    if (i == -1) return "0";
    std::string s = "";
    while (i >= 0) s += std::to_string(result[i--]);

    return s;
}

/// @brief 处理小数点与科学计数法，将传入的字符串转换为纯数字字符串
/// @param s 需要处理的字符串
/// @return 十的次方
int Mul::precision_num(std::string &s) {
    int pre = 0;
    size_t e_pos = s.find_first_of("eE");
    if (e_pos != std::string::npos) {
        pre = std::stoi(s.substr(e_pos + 1));
        s = s.substr(0, e_pos);
    }
    size_t dot_pos = s.find('.');
    if (dot_pos != std::string::npos) {
        int a = s.length() - 1 - dot_pos;
        s = s.erase(dot_pos, 1);
        if(s[0] == '0'){
            s = s.substr(1);;
        }
        return -a + pre;
    }
    return pre;
}

// 高精度加法
inline std::string Mul::addStrings(const std::string &num1, const std::string &num2) {
    std::string result;
    int carry = 0, sum = 0;
    int i = num1.size() - 1, j = num2.size() - 1;
    
    while (i >= 0 || j >= 0 || carry) {
        sum = carry;
        if (i >= 0) sum += num1[i--] - '0';
        if (j >= 0) sum += num2[j--] - '0';
        carry = sum / 10;
        result.push_back(sum % 10 + '0');
    }
    
    std::reverse(result.begin(), result.end());
    return result;
}

//高精度减法
inline std::string Mul::subtractStrings(const std::string &num1, const std::string &num2) {
    std::string result;
    int borrow = 0, diff = 0;
    int i = num1.size() - 1, j = num2.size() - 1;
    
    while (i >= 0 || j >= 0) {
        diff = (i >= 0 ? num1[i--] - '0' : 0) - (j >= 0 ? num2[j--] - '0' : 0) - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.push_back(diff + '0');
    }
    
    // 去掉前导零
    while (result.size() > 1 && result.back() == '0') {
        result.pop_back();
    }
    
    std::reverse(result.begin(), result.end());
    return result;
}

// Karatsuba算法实现
inline std::string Mul::karatsuba(const std::string &num1, const std::string &num2) {

    int n = num1.size();
    int m = num2.size();
    
    if (n == 0 || m == 0) return "0";
    if (n == 1 && m == 1) return std::to_string((num1[0] - '0') * (num2[0] - '0'));
    
    // 使两个数的长度相等
    int max_len = std::max(n, m);
    std::string num1_padded = std::string(max_len - n, '0') + num1;
    std::string num2_padded = std::string(max_len - m, '0') + num2;
    
    int half_len = max_len / 2;
    
    // 分割字符串
    std::string high1 = num1_padded.substr(0, max_len - half_len);
    std::string low1 = num1_padded.substr(max_len - half_len);
    std::string high2 = num2_padded.substr(0, max_len - half_len);
    std::string low2 = num2_padded.substr(max_len - half_len);
    
    // 递归计算
    std::string z0 = karatsuba(low1, low2);
    std::string z1 = karatsuba(addStrings(low1, high1), addStrings(low2, high2));
    std::string z2 = karatsuba(high1, high2);
    
    // 计算结果
    std::string result = addStrings(addStrings(z2 + std::string(2 * half_len, '0'), subtractStrings(z1, addStrings(z2, z0)) + std::string(half_len, '0')), z0);
    
    // 去掉前导零
    while (result.size() > 1 && result[0] == '0') {
        result.erase(result.begin());
    }
    
    return result;
}