/*
 * @Author: your name
 * @Date: 2021-12-05 12:19:48
 * @LastEditTime: 2021-12-13 21:04:15
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \automaton-midterm\DFA.h
 */
#include <iostream>
#include <vector>
#include <map>
#include "MinimizedDFA.h"
#include "FANode.h"
using namespace std;

class DFA
{
public:
    //状态集合，转移函数，开始状态，结束状态集合
    //所有状态用数字命名

    //Variable:
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
                                              //Function
    int outputwith(int in, int posi)
    {
        for (int i = 0; i < transFunc[posi].size(); i++)
        {
            if (transFunc[posi][i].input == in)
                return transFunc[posi][i].output;
        }
        return -1;
    }
    bool interminal(int status)
    {
        for (int i = 0; i < terminalStatus.size(); i++)
        {
            if (status == terminalStatus[i])
                return true;
        }
        return false;
    }
    int finalStatus(int status)
    {
        for (int i = 0; i < RelationStatusList.size(); i++)
        {
            if (RelationStatusList[i].count(status))
                return i;
        }
        return -1;
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
    MinimizedDFA *toMinimizedDFA()
    {
        MinimizedDFA *converted = new MinimizedDFA();
        removeUnrelatedStatus();
        for (int i = 0; i < numOfStatus; i++) //初始化区分表
        {
            vector<int> tmp;
            for (int j = 0; j < numOfStatus; j++)
                tmp.push_back(0);
            DistinguishableTable.push_back(tmp);
        }

        for (int i = 0; i < terminalStatus.size(); i++)
        {
            terminal.insert(terminalStatus[i]);
        }
        for (int i = 0; i < Status.size(); i++) //构造非终结状态表
        {
            if (!terminal.count(Status[i]))
                nonTerminalStatus.push_back(Status[i]);
        }

        for (int i = 0; i < terminalStatus.size(); i++) //初始化F×(Q-F)的可区分状态表
        {
            for (int j = 0; j < nonTerminalStatus.size(); j++)
                DistinguishableTable[terminalStatus[i]][nonTerminalStatus[j]] = DistinguishableTable[nonTerminalStatus[j]][terminalStatus[i]] = 1;
        }

        for (int i = 0; i < terminalStatus.size(); i++) //F×F 递归标记
        {
            for (int j = 0; j < terminalStatus.size(); j++)
            {
                if (i == j)
                    continue;
                bool flag1 = true, flag2 = true;
                int output1 = outputwith(0, terminalStatus[i]);
                int output2 = outputwith(0, terminalStatus[j]);
                int output3 = outputwith(1, terminalStatus[i]);
                int output4 = outputwith(1, terminalStatus[j]);
                if (output1 != -1 && output2 != -1)
                {
                    flag1 = false;
                    if (DistinguishableTable[output1][output2])
                    {
                        //(q,a)(p,a)已被标记
                        DistinguishableTable[nonTerminalStatus[i]][nonTerminalStatus[j]] = DistinguishableTable[nonTerminalStatus[j]][nonTerminalStatus[i]] = 1;
                        for (int k = 0; k < RelationList.size(); k++)
                        {
                            if (RelationList[k].count(make_pair(nonTerminalStatus[i], nonTerminalStatus[j])) || RelationList[k].count(make_pair(nonTerminalStatus[j], nonTerminalStatus[i])))
                            {
                                set<pair<int, int>>::iterator it = RelationList[k].begin();
                                while (it != RelationList[k].end())
                                {
                                    pair<int, int> tmp = *it;
                                    DistinguishableTable[tmp.first][tmp.second] = DistinguishableTable[tmp.second][tmp.first] = 1;
                                    it++;
                                }
                            }
                        }
                    }
                }
                if (output3 != -1 && output4 != -1)
                {
                    flag2 = false;
                    if (DistinguishableTable[output3][output4])
                    {
                        //(q,a)(p,a)已被标记
                        DistinguishableTable[nonTerminalStatus[i]][nonTerminalStatus[j]] = DistinguishableTable[nonTerminalStatus[j]][nonTerminalStatus[i]] = 1;
                        for (int k = 0; k < RelationList.size(); k++)
                        {
                            if (RelationList[k].count(make_pair(nonTerminalStatus[i], nonTerminalStatus[j])) || RelationList[k].count(make_pair(nonTerminalStatus[j], nonTerminalStatus[i])))
                            {
                                set<pair<int, int>>::iterator it = RelationList[k].begin();
                                while (it != RelationList[k].end())
                                {
                                    pair<int, int> tmp = *it;
                                    DistinguishableTable[tmp.first][tmp.second] = DistinguishableTable[tmp.second][tmp.first] = 1;
                                    it++;
                                }
                            }
                        }
                    }
                }
                if (flag1 && flag2)
                {
                    if (output1 != output2 && (terminalStatus[i] != output1 && terminalStatus[j] != output2))
                    {
                        bool flag = true;
                        for (int k = 0; k < RelationList.size(); k++)
                        {
                            if (RelationList[k].count(make_pair(output1, output2)) || RelationList[k].count(make_pair(output2, output1)))
                            {
                                RelationList[k].insert(make_pair(terminalStatus[i], terminalStatus[j]));
                                flag = false;
                            }
                        }
                        if (flag)
                        {
                            set<pair<int, int>> tmp;
                            tmp.insert(make_pair(terminalStatus[i], terminalStatus[j]));
                            tmp.insert(make_pair(output1, output2));
                            RelationList.push_back(tmp);
                        }
                    }
                    if (output3 != output4 && (terminalStatus[i] != output3 && terminalStatus[j] != output4))
                    {
                        bool flag = true;
                        for (int k = 0; k < RelationList.size(); k++)
                        {
                            if (RelationList[k].count(make_pair(output3, output4)) || RelationList[k].count(make_pair(output4, output3)))
                            {
                                RelationList[k].insert(make_pair(terminalStatus[i], terminalStatus[j]));
                                flag = false;
                            }
                        }
                        if (flag)
                        {
                            set<pair<int, int>> tmp;
                            tmp.insert(make_pair(terminalStatus[i], terminalStatus[j]));
                            tmp.insert(make_pair(output3, output4));
                            RelationList.push_back(tmp);
                        }
                    }
                }
            }
        }
        for (int i = 0; i < nonTerminalStatus.size(); i++) //Q-F ×Q-F 递归标记
        {
            for (int j = 0; j < nonTerminalStatus.size(); j++)
            {
                if (i == j)
                    continue;
                int output1 = outputwith(0, nonTerminalStatus[i]);
                int output2 = outputwith(0, nonTerminalStatus[j]);
                int output3 = outputwith(1, nonTerminalStatus[i]);
                int output4 = outputwith(1, nonTerminalStatus[j]);
                if (output1 != -1 && output2 != -1)
                {
                    if (DistinguishableTable[output1][output2])
                    {
                        //(q,a)(p,a)已被标记
                        DistinguishableTable[nonTerminalStatus[i]][nonTerminalStatus[j]] = DistinguishableTable[nonTerminalStatus[j]][nonTerminalStatus[i]] = 1;
                        for (int k = 0; k < RelationList.size(); k++)
                        {
                            if (RelationList[k].count(make_pair(nonTerminalStatus[i], nonTerminalStatus[j])) || RelationList[k].count(make_pair(nonTerminalStatus[j], nonTerminalStatus[i])))
                            {
                                set<pair<int, int>>::iterator it = RelationList[k].begin();
                                while (it != RelationList[k].end())
                                {
                                    pair<int, int> tmp = *it;
                                    DistinguishableTable[tmp.first][tmp.second] = DistinguishableTable[tmp.second][tmp.first] = 1;
                                    it++;
                                }
                            }
                        }
                    }
                }
                else if (output3 != -1 && output4 != -1)
                {
                    if (DistinguishableTable[output3][output4])
                    {
                        //(q,a)(p,a)已被标记
                        DistinguishableTable[nonTerminalStatus[i]][nonTerminalStatus[j]] = DistinguishableTable[nonTerminalStatus[j]][nonTerminalStatus[i]] = 1;
                        for (int k = 0; k < RelationList.size(); k++)
                        {
                            if (RelationList[k].count(make_pair(nonTerminalStatus[i], nonTerminalStatus[j])) || RelationList[k].count(make_pair(nonTerminalStatus[j], nonTerminalStatus[i])))
                            {
                                set<pair<int, int>>::iterator it = RelationList[k].begin();
                                while (it != RelationList[k].end())
                                {
                                    pair<int, int> tmp = *it;
                                    DistinguishableTable[tmp.first][tmp.second] = DistinguishableTable[tmp.second][tmp.first] = 1;
                                    it++;
                                }
                            }
                        }
                    }
                }
                else
                {
                    if (output1 != output2 && (nonTerminalStatus[i] != output1 && nonTerminalStatus[j] != output2))
                    {
                        bool flag = true;
                        for (int k = 0; k < RelationList.size(); k++)
                        {
                            if (RelationList[k].count(make_pair(output1, output2)) || RelationList[k].count(make_pair(output2, output1)))
                            {
                                RelationList[k].insert(make_pair(nonTerminalStatus[i], nonTerminalStatus[j]));
                                flag = false;
                            }
                        }
                        if (flag)
                        {
                            set<pair<int, int>> tmp;
                            tmp.insert(make_pair(nonTerminalStatus[i], nonTerminalStatus[j]));
                            tmp.insert(make_pair(output1, output2));
                            RelationList.push_back(tmp);
                        }
                    }
                    if (output3 != output4 && (nonTerminalStatus[i] != output3 && nonTerminalStatus[j] != output4))
                    {
                        bool flag = true;
                        for (int k = 0; k < RelationList.size(); k++)
                        {
                            if (RelationList[k].count(make_pair(output3, output4)) || RelationList[k].count(make_pair(output4, output3)))
                            {
                                RelationList[k].insert(make_pair(nonTerminalStatus[i], nonTerminalStatus[j]));
                                flag = false;
                            }
                        }
                        if (flag)
                        {
                            set<pair<int, int>> tmp;
                            tmp.insert(make_pair(nonTerminalStatus[i], nonTerminalStatus[j]));
                            tmp.insert(make_pair(output3, output4));
                            RelationList.push_back(tmp);
                        }
                    }
                }
            }
        }

        for (int i = 0; i < DistinguishableTable.size(); i++)
        {
            for (int j = 0; j < DistinguishableTable.size(); j++)
            {
                if (DistinguishableTable[i][j])
                    continue;
                else
                {
                    bool flag = true;
                    for (int k = 0; k < RelationStatusList.size(); k++)
                    {
                        if (RelationStatusList[k].count(i) || RelationStatusList[k].count(j))
                        {
                            RelationStatusList[k].insert(i);
                            RelationStatusList[k].insert(j);
                            flag = false;
                        }
                    }
                    if (flag)
                    {
                        set<int> tmp;
                        tmp.insert(i);
                        tmp.insert(j);
                        RelationStatusList.push_back(tmp);
                    }
                }
            }
        }

        set<int> StatusLeaved;
        for (int i = 0; i < Status.size(); i++)
            StatusLeaved.insert(Status[i]);
        for (int i = 0; i < RelationStatusList.size(); i++)
        {
            set<int>::iterator it = RelationStatusList[i].begin();
            while (it != RelationStatusList[i].end())
            {
                int tmp = *it;
                StatusLeaved.erase(tmp);
                it++;
            }
        }
        RelationStatusList.push_back(StatusLeaved); //状态分类完成
        vector<vector<FANode>> newTransFunc;
        //开始正式转化
        converted->numOfStatus = RelationStatusList.size(); //状态数
        for (int i = 0; i < RelationStatusList.size(); i++)
        { //结束状态
            set<int>::iterator it = RelationStatusList[i].begin();
            while (it != RelationStatusList[i].end())
            {
                int tmp = *it;
                if (interminal(tmp))
                {
                    converted->terminalStatus.push_back(i);
                    break;
                }
                it++;
            }
        }
        for (int i = 0; i < RelationStatusList.size(); i++)
        { //开始状态
            bool flag = false;
            set<int>::iterator it = RelationStatusList[i].begin();
            while (it != RelationStatusList[i].end())
            {
                int tmp = *it;
                if (tmp == this->beginStatus)
                {
                    converted->beginStatus = i;
                    flag = true;
                    break;
                }
                it++;
            }
            if (flag)
                break;
        }

        for (int i = 0; i < converted->numOfStatus; i++)
        {
            vector<FANode> tmp1;
            bool flag1 = false, flag2 = false;
            set<int>::iterator it = RelationStatusList[i].begin();
            while (it != RelationStatusList[i].end())
            {
                int tmp2 = *it;
                if (outputwith(0, tmp2) != -1 && !flag1)
                {
                    flag1 = true;
                    FANode tmp3{0, finalStatus(outputwith(0, tmp2))};
                    tmp1.push_back(tmp3);
                }
                if (outputwith(1, tmp2) != -1 && !flag2)
                {
                    flag2 = true;
                    FANode tmp3{1, finalStatus(outputwith(1, tmp2))};
                    tmp1.push_back(tmp3);
                }
                it++;
            }
            converted->transFunc.push_back(tmp1);
        }
        for (int i = 0; i < converted->numOfStatus; i++)
        {

            converted->Status.push_back(i);
        }
        converted->removeUnrelatedStatus();
        return converted;
    }
    void mydisplay()
    {
        cout << endl;
        for (int i = 0; i < DistinguishableTable.size(); i++)
        {
            for (int j = 0; j < DistinguishableTable.size(); j++)
            {
                cout << DistinguishableTable[i][j] << " ";
            }
            cout << endl;
        }
        for (int i = 0; i < RelationList.size(); i++)
        {
            set<pair<int, int>>::iterator it = RelationList[i].begin();
            while (it != RelationList[i].end())
            {
                cout << "(" << it->first << " " << it->second << ") ";
                it++;
            }
            cout << endl;
        }
        for (int i = 0; i < RelationStatusList.size(); i++)
        {
            set<int>::iterator it = RelationStatusList[i].begin();
            while (it != RelationStatusList[i].end())
            {
                cout << *it << " ";
                it++;
            }
            cout << endl;
        }
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
