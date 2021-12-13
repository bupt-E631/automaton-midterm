/*
 * @Author: your name
 * @Date: 2021-12-05 12:20:33
 * @LastEditTime: 2021-12-13 15:42:42
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \automaton-midterm\MinimizedDFA.h
 */
#include <vector>
using namespace std;

class MinimizedDFA
{
public:
    //状态集合，转移函数，开始状态，结束状态集合
    //所有状态用数字命名

    //Variable:
    int numOfStatus;                  // 状态数量
    int beginStatus;                  // 起始状态
    vector<int> Status;               // 状态
    vector<vector<FANode>> transFunc; // 表
    vector<int> terminalStatus;       // 结束状态

    //Function
    bool interminal(int status)
    {
        for (int i = 0; i < terminalStatus.size(); i++)
        {
            if (status == terminalStatus[i])
                return true;
        }
        return false;
    }
    void removeUnrelatedStatus()
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
    void display()
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
};