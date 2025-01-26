#pragma once
#include <iostream>
#include <variant>
#include <string>
#include <vector>
#include "math/mul.h"
using namespace std;

//complexObject.h
class complexObject {
public:
    string real;
    string virt;
    string res;
    complexObject(string real, string virt) : real(real), virt(virt) {}
    complexObject(string str);
    complexObject() : real("0"), virt("0") {}
    void operator=(const complexObject &c);
    string getRes();
    complexObject operator+(const complexObject &c);
    complexObject operator-(const complexObject &c);
    complexObject operator*(const complexObject &c);
    complexObject operator/(const complexObject &c);
    complexObject operator%(const complexObject &c);
    complexObject operator^(const complexObject &c);
    void operator--(int);
    void operator++(int);
    complexObject operator+=(const complexObject &c);
    complexObject operator-=(const complexObject &c);
    void operator*=(const complexObject &c);
    void operator/=(const complexObject &c);
    friend complexObject con(complexObject &c1);
    friend complexObject mod(complexObject &c1);
};

