#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <queue>
#include <map>
#include <algorithm>
#include<string>
#include "FANode.h"
using namespace std;

class MinimizedDFA
{
public:
    int numOfStatus;                  
    int beginStatus;                  
    vector<int> Status;               
    vector<vector<FANode>> transFunc; 
    vector<int> terminalStatus; 

    MinimizedDFA();

    bool interminal(int);

    void removeUnrelatedStatus();

    void display();
};