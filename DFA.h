#pragma once
#include "MinimizedDFA.h"

class DFA
{
public:
    int numOfStatus;                          // 状态数量
    int beginStatus;                          // 起始状态
    vector<int> Status;                       // 状态
    vector<vector<FANode>> transFunc;         // 表
    vector<int> terminalStatus;               // 结束状态
    vector<vector<int>> DistinguishableTable; //区分表
    vector<set<pair<int, int>>> RelationList; //关联状态对表
    vector<set<int>> RelationStatusList;      //关联状态表
    set<int> terminal;                        //结束状态集合
    vector<int> nonTerminalStatus;            //非结束状态集合

    DFA();

    int outputwith(int, int);

    bool interminal(int);

    int finalStatus(int);

    void removeUnrelatedStatus();

    MinimizedDFA* toMinimizedDFA();

    void mydisplay();

    void display();
};
