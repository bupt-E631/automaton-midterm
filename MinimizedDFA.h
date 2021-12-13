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