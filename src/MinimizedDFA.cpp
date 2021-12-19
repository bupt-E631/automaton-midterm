#include"MinimizedDFA.h"

MinimizedDFA::MinimizedDFA() = default;

bool MinimizedDFA::interminal(int status)
{
    for (int i = 0; i < terminalStatus.size(); i++)
    {
        if (status == terminalStatus[i])
            return true;
    }
    return false;
}

void MinimizedDFA::removeUnrelatedStatus()
{
    set<int> usefulStatus;
    vector<int> visited;
    for (int i = 0; i < numOfStatus; i++)
        visited.push_back(0);
    stack<int> S;
    S.push(beginStatus);
    visited[beginStatus] = 1;
    usefulStatus.insert(beginStatus);
    while (!S.empty())
    {
        int tmp = S.top();
        S.pop();
        for (int i = 0; i < transFunc[tmp].size(); i++)
        {
            if (!visited[transFunc[tmp][i].output])
            {
                visited[transFunc[tmp][i].output] = 1;
                S.push(transFunc[tmp][i].output);
                usefulStatus.insert(transFunc[tmp][i].output);
            }
        }
    }

    vector<vector<FANode>> newtransfunc;
    vector<int> newterminal;
    vector<int> newstatus;
    map<int, int> M;
    int count = 0;
    set<int>::iterator it = usefulStatus.begin();
    while (it != usefulStatus.end())
    {
        int tmp = *it;
        M.insert(make_pair(tmp, count++));
        it++;
    }
    it = usefulStatus.begin();
    while (it != usefulStatus.end())
    {
        newtransfunc.push_back(transFunc[*it]);
        it++;
    }
    it = usefulStatus.begin();
    while (it != usefulStatus.end())
    {
        if (interminal(*it))
            newterminal.push_back(M.at(*it));
        it++;
    }
    for (int i = 0; i < newtransfunc.size(); i++)
    {
        for (int j = 0; j < newtransfunc[i].size(); j++)
        {
            newtransfunc[i][j].output = M.at(newtransfunc[i][j].output);
        }
    }
    numOfStatus = usefulStatus.size();
    for (int i = 0; i < numOfStatus; i++)
    {
        newstatus.push_back(i);
    }
    Status = newstatus;
    transFunc = newtransfunc;
    terminalStatus = newterminal;
}

void MinimizedDFA::display()
{
    cout << "numOfStatus: " << this->numOfStatus << endl;
    cout << "status: ";
    for (auto x : this->Status)
        cout << x << ' ';
    cout << endl;
    cout << "beginStatus: " << this->beginStatus << endl;
    cout << "terminalStatus: ";
    for (auto x : this->terminalStatus)
        cout << x << ' ';
    cout << endl;
    cout << "transfer Functions: " << endl;
    for (int i = 0; i < this->numOfStatus; i++)
    {
        cout << this->Status[i] << ": ";
        if (this->transFunc[i].empty())
        {
            cout << "no transfer function!" << endl;
            continue;
        }
        for (auto x : this->transFunc[i])
        {
            cout << '(' << x.input << ',' << x.output << ')' << ' ';
        }
        cout << endl;
    }
    cout << endl;
}

