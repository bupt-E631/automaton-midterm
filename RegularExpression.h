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
                en.terminalStatus.push_back(1);
                FANode q;
                q.input = 0;
                q.output = 1;
                en.transFunc[0].push_back(q);
                enStack.push(en);
            }
            else if (x == '1') {
                epsilon_NFA en;
                en.beginStatus = 0;
                en.numOfStatus = 2;
                en.Status.push_back(0);
                en.Status.push_back(1);
                en.terminalStatus.push_back(1);
                FANode q;
                q.input = 1;
                q.output = 1;
                en.transFunc[0].push_back(q);
                enStack.push(en);
            }
            //如果是+号，就将栈顶两个epsilon_NFA取出并联，再将结果压入栈中
            else if (x == '+') {
                epsilon_NFA en1 = enStack.top();
                enStack.pop();
                epsilon_NFA en2 = enStack.top();
                enStack.pop();
                epsilon_NFA result = mergeEN(en1, en2);
                enStack.push(result);
            }
            //如果是&号，就将栈顶两个epsilon_NFA取出串联。再将结果压入栈中
            else if (x == '&') {
                epsilon_NFA en1 = enStack.top();
                enStack.pop();
                epsilon_NFA en2 = enStack.top();
                enStack.pop();
                epsilon_NFA result = combineEN(en1, en2);
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
    }


};

//将传入的两台epsilon_NFA并联
epsilon_NFA mergeEN(epsilon_NFA en1, epsilon_NFA en2) {
    epsilon_NFA result;
    return result;
}

//将传入的两台epsilon_NFA串联
epsilon_NFA combineEN(epsilon_NFA en1, epsilon_NFA en2) {
    epsilon_NFA result = en1;
    for (auto x : en2.Status) {
        x += en1.numOfStatus;
        result.Status.push_back(x);
        result.numOfStatus++;
    }
    for (auto x : en2.transFunc) {
        for (auto y : x)
            y.output += en1.numOfStatus;
        result.transFunc.push_back(x);
    }
    result.terminalStatus.clear();
    result.terminalStatus.push_back(result.numOfStatus - 1);
    return result;
}

//将传入的epsilon_NFA闭包化
epsilon_NFA closureEN(epsilon_NFA en) {
    epsilon_NFA result;
    
    return result;
}