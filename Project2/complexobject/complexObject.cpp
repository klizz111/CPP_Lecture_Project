#include <sstream>
#include <stdexcept>
#include "complexObject.h"

complexObject::complexObject(string str) {
    size_t plusPos = str.find('+');
    size_t minusPos = str.find('-', 1); // 从索引1开始查找，以避免负号被误认为是减号
    size_t iPos = str.find('i');

    // 处理纯实数
    if (iPos == string::npos) {
        real = str;
        virt = "0";
    } 
    // 处理纯虚数
    else if (plusPos == string::npos && minusPos == string::npos) {
        real = "0";
        virt = str.substr(0, iPos);
    }
    // 处理包含实部和虚部的复数
    else {
        if (plusPos != string::npos) {
            real = str.substr(0, plusPos);
            virt = str.substr(plusPos + 1, iPos - plusPos - 1);
        } else {
            real = str.substr(0, minusPos);
            virt = str.substr(minusPos, iPos - minusPos);
        }
    }

    // 如果虚部为空，则设置为 "1"
    if (virt.empty()) {
        virt = "1";
    } else if (virt == "-") {
        virt = "-1";
    }
}

void complexObject::operator=(const complexObject &c)
{
    real = c.real;
    virt = c.virt;
}

string complexObject::getRes()
{
    auto isZero = [](string s) {
        for (auto c : s)
        {
            if (c != '0' && c != '.')
            {
                return false;
            }
        }
        return true;
    };

    if (isZero(real) && isZero(virt))
    {
        return "0";
    }
    else if (isZero(virt))
    {
        return real;
    }
    else if (isZero(real))
    {
        return virt + "i";
    }
    else
    {
        if (virt == "1")
        {
            return real + "+i";
        }
        else if (virt[0] == '-')
        {
            return real + virt + "i";
        }
        else
        {
            return real + "+" + virt + "i";
        }
    }
}

complexObject complexObject::operator+(const complexObject &c)
{
    complexObject res("0", "0");
    res.real = addStrings(real, c.real);
    res.virt = addStrings(virt, c.virt);
    return res;
}

complexObject complexObject::operator-(const complexObject &c)
{
    complexObject res("0", "0");
    res.real = subtractStrings(real, c.real);
    res.virt = subtractStrings(virt, c.virt);
    return res;
}

complexObject complexObject::operator*(const complexObject &c)
{
    complexObject res("0", "0");
    auto real1 = stringMultiple(real, c.real);
    auto real2 = stringMultiple(virt, c.virt);
    auto virt1 = stringMultiple(real, c.virt);
    auto virt2 = stringMultiple(virt, c.real);
    res.real = subtractStrings(real1, real2);
    res.virt = addStrings(virt1, virt2);
    return res; 
}

complexObject complexObject::operator/(const complexObject &c)
{
    if (c.real == "0" && c.virt == "0")
    {
        throw "Error: division by zero";
    }
    else if (c.virt != "0" && virt != "0")
    {
        throw "Error: division by complex is unsupported now";
    }
    else
    {
        complexObject res("0", "0");
        res.real = stringDivide(real, c.real);
        res.virt = "0";
        return res;
    }
}

complexObject complexObject::operator%(const complexObject &c)
{
    if (c.real == "0" && c.virt == "0")
    {
        throw "Error: division by zero";
    }
    else if (c.virt != "0" && virt != "0")
    {
        throw "Error: division by complex is unsupported now";
    }
    else
    {
        complexObject res("0", "0");
        res.real = stringPow(real, c.real);
        res.virt = "0";
        return res;
    }
}

complexObject complexObject::operator^(const complexObject &c)
{   
    throw "Error: power is unsupported now";
}

void complexObject::operator--(int)
{
    real = subtractStrings(real, "1");
}

void complexObject::operator++(int)
{
    real = addStrings(real, "1");
}

complexObject complexObject::operator+=(const complexObject &c)
{
    *this = *this + c;
    return *this;
}

complexObject complexObject::operator-=(const complexObject &c)
{
    *this = *this - c;
    return *this;
}

void complexObject::operator*=(const complexObject &c)
{
    *this = *this * c;
}

void complexObject::operator/=(const complexObject &c)
{
    *this = *this / c;
}

complexObject con(complexObject &c1)
{
    if (c1.virt[0] == '-') {
        c1.virt = c1.virt.substr(1);
    }
    else {
        c1.virt = "-" + c1.virt;
    }
    return c1;
}

complexObject mod(complexObject &c1)
{
    complexObject z("0", "0");
    z.real = stringSqrt(stringPow(c1.real, "2"),stringPow(c1.virt, "2"));
    c1.virt = "0";
    c1.real = z.real;
    return z;
}
