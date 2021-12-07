/*
 * @Author: your name
 * @Date: 2021-12-05 12:19:57
 * @LastEditTime: 2021-12-08 01:27:20
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \automaton-midterm\epsilon_NFA.h
 */
#pragma once
#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <stack>
#include "FANode.h"
#include "NFA.h"
using namespace std;
class epsilon_NFA
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
    epsilon_NFA(){};
    NFA *toNFA()
    {
        NFA *NFA_converted = new NFA();
        NFA_converted->numOfStatus = this->numOfStatus;
        NFA_converted->terminalStatus = this->terminalStatus;
        NFA_converted->beginStatus = this->beginStatus;
        NFA_converted->Status = this->Status;
        int num = this->numOfStatus;
        while (num--)
        {
            vector<FANode> tmp;
            NFA_converted->transFunc.push_back(tmp);
        }
        if (canReachTerminalWithEpsilon(this->beginStatus))
        {
            NFA_converted->terminalStatus.push_back(this->beginStatus);
        }
        int n = this->Status.size();

        for (int i = 0; i < this->transFunc.size(); i++) //扩展每行的转移函数
        {
            vector<int> preEpsilon, postEpsilon;
            set<int> S0, S1, S2, S3;
            preEpsilon = ReachWithEpsilon(i);           //首先得到先通过n个空转移达到的状态集合S
            for (int j = 0; j < preEpsilon.size(); j++) //加入S输入0,1后得到的新状态
            {
                for (int k = 0; k < this->transFunc[preEpsilon[j]].size(); k++) //遍历S的转移函数
                {
                    if (this->transFunc[preEpsilon[j]][k].input == 0 && !S0.count(transFunc[preEpsilon[j]][k].output))
                        S0.insert(transFunc[preEpsilon[j]][k].output);
                    else if (this->transFunc[preEpsilon[j]][k].input == 1 && !S1.count(transFunc[preEpsilon[j]][k].output))
                        S1.insert(transFunc[preEpsilon[j]][k].output);
                }
            }
            set<int>::iterator it = S0.begin();
            while (it != S0.end())
            {
                int tmpStatus = *it;
                vector<int> tmpVec = ReachWithEpsilon(tmpStatus);
                for (int j = 0; j < tmpVec.size(); j++)
                {
                    if (!S2.count(tmpVec[j]))
                    {
                        S2.insert(tmpVec[j]);
                    }
                }
                it++;
            }
            it = S2.begin();
            while (it != S2.end())
            {
                int tmpStatus = *it;
                FANode e{0, tmpStatus};
                NFA_converted->transFunc[i].push_back(e);
                it++;
            }
            it = S1.begin();
            while (it != S1.end())
            {
                int tmpStatus = *it;
                vector<int> tmpVec = ReachWithEpsilon(tmpStatus);
                for (int j = 0; j < tmpVec.size(); j++)
                {
                    if (!S3.count(tmpVec[j]))
                    {
                        S3.insert(tmpVec[j]);
                    }
                }
                it++;
            }
            it = S3.begin();
            while (it != S3.end())
            {
                int tmpStatus = *it;
                FANode e{1, tmpStatus};
                NFA_converted->transFunc[i].push_back(e);
                it++;
            }
        }
        return NFA_converted;
    }
    bool canReachTerminalWithEpsilon(int status)
    {
        queue<int> Q;
        int n = this->Status.size();
        vector<int> visited(n);
        for (int i = 0; i < n; i++)
            visited.push_back(0);
        Q.push(status);
        while (!Q.empty())
        {
            int tmp = Q.front();
            Q.pop();
            int size = this->transFunc[tmp].size();
            for (int i = 0; i < size; i++)
            {
                if (this->transFunc[tmp][i].input == -1 && !visited[this->transFunc[tmp][i].output])
                {
                    if (inTerminal(this->transFunc[tmp][i].output))
                    {
                        return true;
                    }
                    Q.push(this->transFunc[tmp][i].output);
                    visited[this->transFunc[tmp][i].output] = 1;
                }
            }
        }
        return false;
    }
    vector<int> ReachWithEpsilon(int status)
    {
        vector<int> returnVec;
        set<int> S;
        queue<int> Q;
        int n = this->numOfStatus;
        vector<int> visited(n);
        for (int i = 0; i < n; i++)
        {
            visited.push_back(0);
        }

        Q.push(status);
        S.insert(status);
        visited[status] = 1;
        while (!Q.empty())
        {
            int tmpStatus = Q.front();
            Q.pop();
            for (int i = 0; i < this->transFunc[tmpStatus].size(); i++)
            {
                if (this->transFunc[tmpStatus][i].input == -1 && !visited[this->transFunc[tmpStatus][i].output])
                {
                    Q.push(this->transFunc[tmpStatus][i].output);
                    visited[this->transFunc[tmpStatus][i].output] = 1;
                    S.insert(this->transFunc[tmpStatus][i].output);
                }
            }
        }
        set<int>::iterator it = S.begin();
        while (it != S.end())
        {
            returnVec.push_back(*it);
            it++;
        }
        return returnVec;
    }
    bool inTerminal(int status)
    {
        for (int i : this->terminalStatus)
        {
            if (status == i)
                return true;
        }
        return false;
    }
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
