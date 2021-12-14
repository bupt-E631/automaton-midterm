#pragma once
#include "DFA.h"

struct Node
{
public:
    int input;
    string output;
    Node() = default;
    Node(int input, string output)
    {
        this->input = input;
        this->output = output;
    }
};

class NFA
{
public:
    int numOfStatus;
    int beginStatus;
    vector<int> Status;
    vector<set<int>> myStatus;
    vector<vector<FANode>> transFunc;
    vector<vector<FANode>> transFuncBak;
    vector<vector<set<int>>> myTransFunc;
    vector<int> terminalStatus;
    vector<set<int>> myTer;
    vector<int> terminalStatusBak;

    NFA();

    DFA* toDFA(bool);

    void toTerminal();

    void toTransFunc();

    bool isSameSet(set<int>, set<int>);

    void toStatus();

    void addTer();

    void formExtend(set<int>);

    void myDisplay();

    bool exist(set<int>);

    void statusMapping();

    void transFuncConvert();

};