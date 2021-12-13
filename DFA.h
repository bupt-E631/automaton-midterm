/*
 * @Author: your name
 * @Date: 2021-12-05 12:19:48
 * @LastEditTime: 2021-12-13 16:02:41
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \automaton-midterm\DFA.h
 */
#include <iostream>
#include <vector>
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
    MinimizedDFA *toMinimizedDFA()
    {
        MinimizedDFA *converted = new MinimizedDFA();
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
            if (!terminal.find(Status[i]))
                nonTerminalStatus.push_back(Status[i]);
        }
        for (int i = 0; i < terminalStatus.size(); i++) //初始化F×(Q-F)的可区分状态表
        {
            for (int j = 0; j < nonTerminalStatus.size(); j++)
                DistinguishableTable[i][j] = DistinguishableTable[j][i] = 1;
        }
        for (int i = 0; i < terminalStatus.size(); i++) //F×F 递归标记
        {
            for (int j = 0; j < terminalStatus.size(); j++)
            {
                if (i == j)
                    continue;

                int output1 = outputwith(0, terminalStatus[i]);
                int output2 = outputwith(0, terminalStatus[j]);
                int output3 = outputwith(1, terminalStatus[i]);
                int output4 = outputwith(1, terminalStatus[j]);
                if (DistinguishableTable[output1][output2] || DistinguishableTable[output3][output4])
                {
                    //(q,a)(p,a)已被标记
                    DistinguishableTable[i][j] = DistinguishableTable[j][i] = 1;
                    for (int k = 0; k < RelationList.size(); k++)
                    {
                        if (RelationList[k].find(make_pair(i, j)) || RelationList[k].find(make_pair(j, i)))
                        {
                            set<pair>::iterator it = RelationList[k].begin;
                            while (it != RelationList[k].end())
                            {
                                pair<int, int> tmp = *it;
                                DistinguishableTable[tmp.first][tmp.second] = DistinguishableTable[tmp.second][tmp.first] = 1;
                                it++;
                            }
                        }
                    }
                }
                else
                {
                    if (output1 != output2 && (terminalStatus[i] != output1 && terminalStatus[j] != output2))
                    {
                        bool flag = true;
                        for (int k = 0; k < RelationList.size(); k++)
                        {
                            if (RelationList[k].find(make_pair(output1, output2)) || RelationList[k].find(make_pair(output2, output1)))
                            {
                                RelationList[k].insert(make_pair(i, j));
                                flag = false;
                            }
                        }
                        if (flag)
                        {
                            set<pair<int, int>> tmp;
                            tmp.insert(make_pair(i, j));
                            tmp.insert(make_pair(output1, output2));
                            RelationList.push_back(tmp);
                        }
                    }
                    if (output3 != output4 && (terminalStatus[i] != output3 && terminalStatus[j] != output4))
                    {
                        bool flag = true;
                        for (int k = 0; k < RelationList.size(); k++)
                        {
                            if (RelationList[k].find(make_pair(output3, output4)) || RelationList[k].find(make_pair(output4, output3))
                            {
                                RelationList[k].insert(make_pair(i, j));
                                flag = false;
                            }
                        }
                        if (flag)
                        {
                            set<pair<int, int>> tmp;
                            tmp.insert(make_pair(i, j));
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
                if (DistinguishableTable[output1][output2] || DistinguishableTable[output3][output4])
                {
                    //(q,a)(p,a)已被标记
                    DistinguishableTable[i][j] = DistinguishableTable[j][i] = 1;
                    for (int k = 0; k < RelationList.size(); k++)
                    {
                        if (RelationList[k].find(make_pair(i, j)) || RelationList[k].find(make_pair(j, i)))
                        {
                            set<pair>::iterator it = RelationList[k].begin;
                            while (it != RelationList[k].end())
                            {
                                pair<int, int> tmp = *it;
                                DistinguishableTable[tmp.first][tmp.second] = DistinguishableTable[tmp.second][tmp.first] = 1;
                                it++;
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
                            if (RelationList[k].find(make_pair(output1, output2)) || RelationList[k].find(make_pair(output2, output1)))
                            {
                                RelationList[k].insert(make_pair(i, j));
                                flag = false;
                            }
                        }
                        if (flag)
                        {
                            set<pair<int, int>> tmp;
                            tmp.insert(make_pair(i, j));
                            tmp.insert(make_pair(output1, output2));
                            RelationList.push_back(tmp);
                        }
                    }
                    if (output3 != output4 && (nonTerminalStatus[i] != output3 && nonTerminalStatus[j] != output4))
                    {
                        bool flag = true;
                        for (int k = 0; k < RelationList.size(); k++)
                        {
                            if (RelationList[k].find(make_pair(output3, output4)) || RelationList[k].find(make_pair(output4, output3))
                            {
                                RelationList[k].insert(make_pair(i, j));
                                flag = false;
                            }
                        }
                        if (flag)
                        {
                            set<pair<int, int>> tmp;
                            tmp.insert(make_pair(i, j));
                            tmp.insert(make_pair(output3, output4));
                            RelationList.push_back(tmp);
                        }
                    }
                }
            }
        }
        for (int i = 0; i < RelationList.size(); i++)
        {
            set<int, int>::iterator it = RelationList[i].begin();
            set<int> S;
            while (it != RelationList[i].end())
            {
                pair<int, int> tmp = *it;
                int first = tmp.first;
                int second = tmp.second;
                if (!S.find(first))
                    S.insert(first);
                if (!S.find(second))
                    S.insert(second);

                it++;
            }
            RelationStatusList.push_back(S);
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
        converted->numOfStatus = RelationStatusList.size();

        return converted;
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
    int outputwith(int in, int posi)
    {
        for (int i = 0; i < transFunc[posi].size(); i++)
        {
            if (transFunc[posi][i].input == in)
                return transFunc[posi][i].output;
        }
    }
};
