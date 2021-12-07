/*
 * @Author: your name
 * @Date: 2021-12-05 12:20:54
 * @LastEditTime: 2021-12-06 01:52:42
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \automaton-midterm\NFA.h
 */
#pragma once
#include <iostream>
#include <vector>
#include "FANode.h"
using namespace std;
class NFA
{
public:
    //状态集合，转移函数，开始状态，结束状态集合
    //所有状态用数字命名

    //Variable:
    int numOfStatus;
    int beginStatus;
    vector<int> Status;
    vector<vector<FANode>> transFunc;
    vector<int> terminalStatus;
    //Function:
    NFA(){};
    void display()
    {
        for (int i = 0; i < this->transFunc.size(); i++)
        {
            cout << i << ": ";
            for (int j = 0; j < this->transFunc[i].size(); j++)
            {
                cout << "(" << this->transFunc[i][j].input << "," << this->transFunc[i][j].output << ") ";
            }
            cout << endl;
        }
    }
};