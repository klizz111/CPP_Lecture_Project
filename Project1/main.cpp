#include <iostream>
#include <string>
#include "mul.h"

int main(int argc, char* argv[])
{
    //参数检查
    if (argc < 3)
    {
        std::cerr << " Usage: " << argv[0] << " <num1> <num2> [options]" << std::endl;
        std::cerr << " options: -d show operating time" << std::endl;
        std::cerr << "          -p<num> set precision" << std::endl;
        std::cerr << "          -e show result in scientific notation" << std::endl;
        std::cerr << "          -k use Karatsuba algorithm" << std::endl;
        return 1;
    }

    std::string n1,n2;
    n1 = argv[1];
    n2 = argv[2];

    std::vector<std::string> options;
    for (int i = 3; i < argc; i++)
    {
        options.push_back(argv[i]);
    }

    try{
        Mul m(n1,n2,options);   
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
        exit(1);
    }
    return 0;
}