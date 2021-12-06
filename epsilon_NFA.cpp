/*
 * @Author: your name
 * @Date: 2021-12-05 12:12:45
 * @LastEditTime: 2021-12-05 12:12:45
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
    NFA_converted->terminalStatus = this->terminalStatus;
    if ()
}