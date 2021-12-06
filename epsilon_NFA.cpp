/*
 * @Author: your name
 * @Date: 2021-12-05 12:12:45
 * @LastEditTime: 2021-12-07 01:30:40
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \automaton-midterm\epsilon_NFA.cpp
 */
#include "epsilon_NFA.h"
#include "NFA.h"
epsilon_NFA::epsilon_NFA()
{
}
NFA *epsilon_NFA::toNFA()
{
    NFA *NFA_converted = new NFA();
    NFA_converted->numOfStatus = this->numOfStatus;
    NFA_converted->terminalStatus = this->terminalStatus;
    NFA_converted->beginStatus = this->beginStatus;
    NFA_converted->Status = this->Status;
    if (canReachTerminalWithEpsilon(this->beginStatus))
    {
        NFA_converted->terminalStatus.push_back(this->beginStatus);
    }
    for (int i = 1; i <= this->transFunc.capacity(); i++)
    {
    }
}
bool epsilon_NFA::canReachTerminalWithEpsilon(int status)
{
    queue<int> Q;
    int n = this->Status.capacity();
    vector<int> visited(n);
    for (int i = 0; i <= n; i++)
        visited.push_back(0);
    Q.push(status);
    while (!Q.empty())
    {
    }
}
bool epsilon_NFA::inTerminal(int status)
{
    for (int i : this->terminalStatus)
    {
        if (status == i)
            return true;
    }
    return false;
}
