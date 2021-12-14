#pragma once
#include "epsilon_NFA.h"
using namespace std;

class RegularExpression {
public:
    string RE;
    //构造函数，读入RE
    RegularExpression(string);
    //预处理，加入被省略的&符号
    string preProcessing();
    //将RE转化为机器可读的后缀表达式
    string toPost();
    //转化为ε-NFA
    epsilon_NFA to_epsilon_NFA();
};