/*
 * @Author: your name
 * @Date: 2021-12-05 12:20:43
 * @LastEditTime: 2021-12-05 12:20:43
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \automaton-midterm\RegularExpression.h
 */
#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include<string>
#include "FANode.h"
#include "epsilon_NFA.h"
#define MAX_LENGTH 200
using namespace std;

//下面是三个函数，分别在遇到+、&和*时执行，用于对栈顶的一个或两个NFA进行合并

//将传入的两台epsilon_NFA并联
epsilon_NFA mergeEN(epsilon_NFA en1, epsilon_NFA en2) {
    epsilon_NFA result;
    result.numOfStatus = 0;
    //创建0号状态，并将其通过空转移移向两台要合并的机子的起始状态
    vector<FANode> resultBegin;
    resultBegin.push_back(FANode(-1, 1));
    resultBegin.push_back(FANode(-1, 1 + en1.numOfStatus));
    //将0号状态及其转移函数集插入结果
    result.Status.push_back(0);
    result.numOfStatus++;
    result.transFunc.push_back(resultBegin);
    //将一号NFA的状态号加一并插入结果状态集
    for (auto& x : en1.Status) {
        x++;
        result.Status.push_back(x);
        result.numOfStatus++;
    }
    //将1号NFA的转移函数的结果状态号加一并插入结果转移函数集
    for (auto& x : en1.transFunc) {
        for (auto& y : x)
            y.output += 1;
        result.transFunc.push_back(x);
    }
    //将2号NFA的状态号加上（1+en1.numOfStatus)并插入结果状态集
    for (auto& x : en2.Status) {
        x += (1 + en1.numOfStatus);
        result.Status.push_back(x);
        result.numOfStatus++;
    }
    //将2号机的转移函数的结果状态号加上（1+en1.numOfStatus)并插入结果转移函数集
    for (auto& x : en2.transFunc) {
        for (auto& y : x)
            y.output += (1 + en1.numOfStatus);
        result.transFunc.push_back(x);
    }
    //将一个终止状态插入结果状态集
    result.Status.push_back(1 + en1.numOfStatus + en2.numOfStatus);
    result.numOfStatus++;
    //将原来两台NFA的终止状态通过空转移指向结果的终止状态
    result.transFunc[en1.numOfStatus].push_back(FANode(-1, 1 + en1.numOfStatus + en2.numOfStatus));
    result.transFunc[en1.numOfStatus + en2.numOfStatus].push_back(FANode(-1, 1 + en1.numOfStatus + en2.numOfStatus));
    //插入终止状态的转移函数集（空的）
    vector<FANode> resultEnd;
    result.transFunc.push_back(resultEnd);
    //将结果的终止状态设为最后一个状态
    result.terminalStatus.clear();
    result.terminalStatus.push_back(result.numOfStatus - 1);
    return result;
}

//将传入的两台epsilon_NFA串联
epsilon_NFA combineEN(epsilon_NFA en1, epsilon_NFA en2) {
    //让结果NFA先等于原1号NFA
    epsilon_NFA result = en1;
    //将en2的状态插入result中
    for (auto& x : en2.Status) {
        //首先将状态号调整好
        x += en1.numOfStatus;
        //然后将状态插入result的状态中
        result.Status.push_back(x);
        //result状态数自增一
        result.numOfStatus++;
    }
    //使原1号NFA的最后一个状态能通过一个空转移移向2号NFA的起始状态
    result.transFunc[en1.numOfStatus - 1].push_back(FANode(-1, en1.numOfStatus));
    //将2号NFA的转移函数插入result中
    for (auto& x : en2.transFunc) {
        for (auto& y : x)
            //首先将转移结果的状态的状态号调整好
            y.output += en1.numOfStatus;
        //将装有转移函数的vector插入result
        result.transFunc.push_back(x);
    }
    //调整终止状态为原2号NFA的终止状态
    result.terminalStatus.clear();
    result.terminalStatus.push_back(result.numOfStatus - 1);
    return result;
}

//将传入的epsilon_NFA闭包化
epsilon_NFA closureEN(epsilon_NFA en) {
    epsilon_NFA result;
    //由于闭包化是要在起始和结尾各增加一个状态，所以让结果的状态数为原NFA的+2
    result.numOfStatus = en.numOfStatus + 2;
    //让原结束状态能经过空转移能到达原起始状态
    en.transFunc[en.numOfStatus - 1].push_back(FANode(-1, 0));
    vector<FANode> resultBegin;
    //向新的起始状态的转移函数中加入空转移到1号和终止状态的结点
    resultBegin.push_back(FANode(-1, 1));
    resultBegin.push_back(FANode(-1, en.numOfStatus + 1));
    //将结果NFA的起始状态号设为0
    result.beginStatus = 0;
    //将状态0插入结果的状态集
    result.Status.push_back(0);
    //将0号状态的转移函数插入result
    result.transFunc.push_back(resultBegin);
    //将原NFA的状态插入结果的状态集
    for (auto& x : en.Status) {
        //首先调整状态号，由于只在起始状态前插入了一个状态，状态号加一即可
        x++;
        //将状态插入结果的状态集
        result.Status.push_back(x);
        //由于开始已经设置好了结果的状态数，所以这里不用让其自增一
    }
    //将原状态转移函数的状态号修正后，将状态转移函数插入结果中
    for (auto& x : en.transFunc) {
        for (auto& y : x)
            //每个转移函数的结果状态号加一
            y.output++;
        //将转移状态集插入结果
        result.transFunc.push_back(x);
    }
    //插入一个终止状态，其状态转移函数为空
    result.Status.push_back(result.numOfStatus - 1);
    vector<FANode> resultEnd;
    result.transFunc.push_back(resultEnd);
    //让原NFA的终止状态能够通过空转移移动到结果的终止状态
    result.transFunc[result.numOfStatus - 2].push_back(FANode(-1, result.numOfStatus - 1));
    //将结果的终止状态设为最后一个状态
    result.terminalStatus.clear();
    result.terminalStatus.push_back(result.numOfStatus - 1);
    return result;
}

class RegularExpression {
public:
    string RE;
    //构造函数，读入RE
    RegularExpression(string RE_) {
        this->RE = RE_;
    }
    //预处理，加入被省略的&符号
    string preProcessing() {
        string temp = "";
        for (int i = 0;i < this->RE.size();i++)
            if ((this->RE[i] == '0' || this->RE[i] == '1' || this->RE[i] == '*' || this->RE[i] == ')') &&
                (this->RE[i + 1] == '0' || this->RE[i + 1] == '1' || this->RE[i + 1] == '('))
                temp = temp + this->RE[i] + '&';
            else
                temp += this->RE[i];
        return temp;
    }
    //将RE转化为机器可读的后缀表达式
    string toPost() {
        string temp = this->preProcessing();
        string post_RE = "";
        stack<char> charStack;
        for (auto x : temp) {
            if (x == '0' || x == '1')
                post_RE += x;
            else if (x == '(')
                charStack.push(x);
            else if (x == ')')
                while (!charStack.empty()) {
                    if (charStack.top() != '(') {
                        post_RE += charStack.top();
                        charStack.pop();
                    }
                    else {
                        charStack.pop();
                        break;
                    }
                }
            else if (x == '*') {
                while (!charStack.empty()) {
                    if (charStack.top() == '*') {
                        post_RE += charStack.top();
                        charStack.pop();
                    }
                    else
                        break;
                }
                charStack.push(x);
            }
            else if (x == '&') {
                while (!charStack.empty()) {
                    if (charStack.top() == '*' || charStack.top() == '&') {
                        post_RE += charStack.top();
                        charStack.pop();
                    }
                    else
                        break;
                }
                charStack.push(x);
            }
            else if (x == '+') {
                while (!charStack.empty()) {
                    if (charStack.top() == '*' || charStack.top() == '&' || charStack.top() == '+') {
                        post_RE += charStack.top();
                        charStack.pop();
                    }
                    else
                        break;
                }
                charStack.push(x);
            }
        }
        while (!charStack.empty()) {
            post_RE += charStack.top();
            charStack.pop();
        }
        return post_RE;
    }
    //转化为ε-NFA
    epsilon_NFA to_epsilon_NFA() {
        string post_RE = this->toPost();
        stack<epsilon_NFA> enStack;
        for (auto x : post_RE) {
            //如果是操作数，就建立一个简单的epsilon_NFA并压栈
            if (x == '0') {
                epsilon_NFA en;
                en.beginStatus = 0;
                en.numOfStatus = 2;
                en.Status.push_back(0);
                en.Status.push_back(1);
                vector<FANode> p;
                en.transFunc.push_back(p);
                en.transFunc.push_back(p);
                en.terminalStatus.push_back(1);
                en.transFunc[0].push_back(FANode(0, 1));
                enStack.push(en);
            }
            else if (x == '1') {
                epsilon_NFA en;
                en.beginStatus = 0;
                en.numOfStatus = 2;
                en.Status.push_back(0);
                en.Status.push_back(1);
                vector<FANode> p;
                en.transFunc.push_back(p);
                en.transFunc.push_back(p);
                en.terminalStatus.push_back(1);
                en.transFunc[0].push_back(FANode(1, 1));
                enStack.push(en);
            }
            //如果是+号，就将栈顶两个epsilon_NFA取出并联，再将结果压入栈中
            else if (x == '+') {
                epsilon_NFA en1 = enStack.top();
                enStack.pop();
                epsilon_NFA en2 = enStack.top();
                enStack.pop();
                epsilon_NFA result = mergeEN(en2, en1);
                enStack.push(result);
            }
            //如果是&号，就将栈顶两个epsilon_NFA取出串联。再将结果压入栈中
            else if (x == '&') {
                epsilon_NFA en1 = enStack.top();
                enStack.pop();
                epsilon_NFA en2 = enStack.top();
                enStack.pop();
                epsilon_NFA result = combineEN(en2, en1);
                enStack.push(result);
            }
            //如果是*号，就将栈顶epsilon_NFA取出并闭包化。再将结果压入栈中
            else if (x == '*') {
                epsilon_NFA en1 = enStack.top();
                enStack.pop();
                epsilon_NFA result = closureEN(en1);
                enStack.push(result);
            }
        }
        return enStack.top();
    }
};
