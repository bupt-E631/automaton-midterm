#pragma once
#include "NFA.h"
class epsilon_NFA
{
public:
    int numOfStatus;

    int beginStatus;

    vector<int> Status;

    vector<vector<FANode>> transFunc;

    vector<int> terminalStatus;

    epsilon_NFA();

    NFA* toNFA();

    bool canReachTerminalWithEpsilon(int);

    vector<int> ReachWithEpsilon(int);

    bool inTerminal(int);

    void display();
};
