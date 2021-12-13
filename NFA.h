/*
 * @Author: your name
 * @Date: 2021-12-05 12:20:54
 * @LastEditTime: 2021-12-13 16:51:09
 * @LastEditors: Karigen B
 * @Description: 完成NFA到DFA的转换
 * @FilePath: \automaton-midterm\NFA.h
 */

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include "FANode.h"
#include "DFA.h"
using namespace std;

class Node
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
    //状态集合，转移函数，开始状态，结束状态集合
    //所有状态用数字命名

    //Variable:
    int numOfStatus;    // 状态数量
    int beginStatus;    // 起始状态
    vector<int> Status; // 状态
    vector<set<int>> myStatus;
    vector<vector<FANode>> transFunc; // 表
    vector<vector<FANode>> transFuncBak;
    vector<vector<set<int>>> myTransFunc;

    vector<int> terminalStatus; // 结束状态
    vector<set<int>> myTer;
    vector<int> terminalStatusBak; // 结束状态
    NFA(){

    };
    //Function:

    
    DFA* toDFA(bool isThis){
        transFuncConvert();//垃圾架构转换为我的架构
        statusMapping();//垃圾架构状态向量转换为我的向量

        //根据我的表去做转换
        for (int i = 0; i < myTransFunc.size(); i++)
        {
            for (int j = 0; j < myTransFunc[i].size(); j++)
            {
                formExtend(myTransFunc[i][j]);
            }
        }

        //加Terminal
        addTer();
        
        //myStatus是转换的关键

        //将我的状态映射将垃圾架构的映射改了
        toStatus();

        //根据我的映射状态在去转换为垃圾表
        toTransFunc();

        //将我的结束状态转换为垃圾结束状态
        toTerminal();

        DFA* dfa = new DFA();
        dfa->beginStatus = this->beginStatus;
        dfa->numOfStatus = this->numOfStatus;
        dfa->Status = this->Status;
        dfa->transFunc = this->transFunc;
        dfa->terminalStatus = this->terminalStatus;

        return dfa;
    }

    void toTerminal(){
        //清空原结束状态表
        terminalStatus.clear();
        for (int i = 0; i < myTer.size(); i++) {
            //用我的结束状态表对照myStatus找位置存入
            for (int j = 0; j < Status.size(); j++) {//状态位置
                if(isSameSet(myTer[i], myStatus[j])){
                    terminalStatus.push_back(j);
                }
            }
            
        }
        
    }

    void toTransFunc(){
        transFunc.clear();
        //表先建起来
        for (int i = 0; i < myTransFunc.size(); i++) {
            transFunc.push_back(vector<FANode>());
        }
        
        //修改数据
        for (int i = 0; i < myTransFunc.size(); i++) {
            //输入0
            if(myTransFunc[i][0].size() == 0){
                //空集合,说明不接受,直接跳过
            } else {//接收
                for (int j = 0; j < Status.size(); j++) {
                    if(isSameSet(myTransFunc[i][0], myStatus[j])){
                        transFunc[i].push_back(FANode(0, j));
                    }
                }
                
            }
            //输入1
            if(myTransFunc[i][1].size() != 0){
                for (int j = 0; j < Status.size(); j++) {
                    if(isSameSet(myTransFunc[i][1], myStatus[j])){
                        transFunc[i].push_back(FANode(1, j));
                    }
                }
            }
        }
        
    }

    bool isSameSet(set<int> s0, set<int> s1){
        if(s0.size() != s1.size()){
            return false;
        } else {
            set<int> temp;
            set_symmetric_difference(s0.begin(), s0.end(), s1.begin(), s1.end(), inserter(temp, temp.begin()));
            return temp.size() == 0;
        }
    }

    void toStatus(){
        for (int i = Status.size(); i < myStatus.size(); i++) {
            //根据我的映射表将Status修改过来
            Status.push_back(i);
        }
        
    }

    void addTer(){
        for (int i = 0; i < myStatus.size(); i++) {
            for (int j = 0; j < terminalStatus.size(); j++) {//如果包含原生结束状态,说明此状态是结束状态
                if(myStatus[i].count(terminalStatus[j]) != 0){
                    myTer.push_back(myStatus[i]);
                }
            }
        }
    }

    void formExtend(set<int> unit)
    {
        //先判断这个状态是否为空
        if (unit.size() == 0)
        {
            return;
        }

        //判断该状态是否包含于状态表
        if (exist(unit))
        {
            return;
        }

        // 该集合不包含于状态表,是新集合
        //加入状态表
        myStatus.push_back(unit);
        //表中创建新列 --最后一行存放新状态
        vector<set<int>> temp;
        temp.push_back(set<int>());
        temp.push_back(set<int>());
        myTransFunc.push_back(temp);
        numOfStatus++;
        //确定输出状态 --新状态实在原生状态的基础上合并得来的,所以各个元素肯定是原原生状态
        set<int> buffer0;
        set<int> buffer1;
        for (set<int>::iterator iter = unit.begin(); iter != unit.end(); iter++)
        {
            //输入0
            set<int> temp0;
            //myTransFunc[*iter][0]; --为对应集合
            set_union(myTransFunc[*iter][0].begin(), myTransFunc[*iter][0].end(), buffer0.begin(), buffer0.end(), inserter(temp0, temp0.begin()));
            buffer0.clear();
            buffer0 = temp0;
            //输入1
            set<int> temp1;
            //myTransFunc[*iter][1];
            set_union(myTransFunc[*iter][1].begin(), myTransFunc[*iter][1].end(), buffer1.begin(), buffer1.end(), inserter(temp1, temp1.begin()));
            buffer1.clear();
            buffer1 = temp1;
        }
        //放入对应行
        myTransFunc[myTransFunc.size() - 1][0] = buffer0;
        myTransFunc[myTransFunc.size() - 1][1] = buffer1;
    }

    void myDisplay()
    {
        for (int i = 0; i < myStatus.size(); i++)
        {
            //输表头
            for (set<int>::iterator iter = myStatus[i].begin(); iter != myStatus[i].end(); iter++)
            {
                cout << *iter;
            }
            cout << ":";
            for (set<int>::iterator iter = myTransFunc[i][0].begin(); iter != myTransFunc[i][0].end(); iter++)
            {
                cout << *iter;
            }
            cout << "  ";
            for (set<int>::iterator iter = myTransFunc[i][1].begin(); iter != myTransFunc[i][1].end(); iter++)
            {
                cout << *iter;
            }
            cout << endl;
        }
    }

    bool exist(set<int> unit)
    {
        for (int i = 0; i < myStatus.size(); i++)
        {
            if (unit.size() != myStatus[i].size())
            { //大小不同肯定不是一个状态
                continue;
            }
            else
            { // 集合个数相等的情况下判断是否是同一个集合
                //根据对称差集是否为空判断集合是否相等
                set<int> temp;
                set_symmetric_difference(unit.begin(), unit.end(), myStatus[i].begin(), myStatus[i].end(), inserter(temp, temp.begin()));
                if (temp.size() == 0)
                {
                    return true;
                }
            }
        }

        return false;
    }

    void statusMapping()
    {
        for (int i = 0; i < Status.size(); i++)
        {
            set<int> temp;
            temp.insert(Status[i]);
            myStatus.push_back(temp);
        }
    }

    void transFuncConvert()
    {
        //1.先构建空表
        vector<set<int>> temp;
        temp.push_back(set<int>());
        temp.push_back(set<int>());
        for (int i = 0; i < numOfStatus; i++)
        {
            myTransFunc.push_back(temp);
        }
        //2.将数据放入空表
        for (int i = 0; i < transFunc.size(); i++)
        {
            for (int j = 0; j < transFunc[i].size(); j++)
            {
                if (transFunc[i][j].input == 0)
                {
                    myTransFunc[i][0].insert(transFunc[i][j].output); //将输入1转换过去的状态放入对应位置
                }
                else if (transFunc[i][j].input == 1)
                {
                    myTransFunc[i][1].insert(transFunc[i][j].output); //将输入1转换过去的状态放入对应位置
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    DFA *toDFA()
    {
        transFuncBak = transFunc;
        terminalStatusBak = terminalStatus;
        //
        //for (int i = 0; i < transFunc.size(); i++) {
        //    myTransFunc.push_back(vector<Node>());
        //}

        FANodeConvert();

        // 测试代码
        for (int i = 0; i < myTransFunc.size(); i++)
        {
            for (int j = 0; j < myTransFunc[i].size(); j++)
            {
                //cout << myTransFunc[i][j].input << "->" << myTransFunc[i][j].output << endl;
            }
            cout << endl;
        }

        // NFA -> DFA
        for (int i = 0; i < transFunc.size(); i++)
        { // 外层循环,状态数
            for (int j = 0; j < transFunc[i].size(); j++)
            { // 内层循环,0/1的输入状态方程
                FANodeExtend(transFunc[i][j]);
            }
        }

        //convert();

        // 返回

        DFA *dfa = new DFA();
        dfa->beginStatus = this->beginStatus;
        dfa->numOfStatus = this->numOfStatus;
        dfa->Status = this->Status;
        dfa->terminalStatus = this->terminalStatus;
        dfa->transFunc = this->transFunc;
        return dfa;
    }

    void FANodeExtend(FANode node)
    {
        //
        for (int i = 0; i < Status.size(); i++)
        {
            if (Status[i] == node.output)
            {
                return;
            }
        }

        //构建新的一行
        Status.push_back(node.output); //放入状态表
        numOfStatus++;
        transFunc.push_back(vector<FANode>());

        //处理新状态
        int count = to_string(node.output).length(); //需要比较的次数
        int is = 0;                                  //要分析的状态
        for (int k = 0; k <= 1; k++)
        { //输入0或1
            is = node.output;
            set<int, greater<int>> SSet;
            for (int i = 0; i < count; i++)
            {
                int tis = is % 10;
                for (int j = 0; j < transFuncBak[tis].size(); j++)
                {
                    if (transFuncBak[tis][j].input == k)
                    { // 0或1
                        SSet.insert(transFuncBak[tis][j].output);
                    }
                }

                is /= 10;
            }

            if (SSet.size() == 0)
            {
                continue;
            }

            bool isTer = false;
            for (int i = 0; i < terminalStatusBak.size() && !isTer; i++)
            {
                if (SSet.count(terminalStatusBak[i]))
                {
                    isTer = true;
                }
            }

            if (isTer)
            {
                terminalStatus.push_back(node.output);
            }

            //生成新状态的输出状态
            int rs = 0; //新状态
            for (set<int>::iterator iter = SSet.begin(); iter != SSet.end(); iter++)
            {
                rs *= 10;
                rs += *iter;
            }
            transFunc[transFunc.size() - 1].push_back(FANode(k, rs));
        }
    }

    void FANodeConvert()
    {
        for (int i = 0; i < transFunc.size(); i++)
        {
            bool f0 = false;
            bool f1 = false;
            for (int j = 0; j < transFunc[i].size() - 1; j++)
            {
                if (transFunc[i][j].input == 0)
                {
                    f0 = true;
                }
                else if (transFunc[i][j].input == 1)
                {
                    f1 = true;
                }
            }

            int in0 = 0;
            int in1 = 0;
            string s0 = "";
            string s1 = "";
            for (int j = transFunc[i].size() - 1; j >= 0; j--)
            { // 整数最高位不能是0
                if (transFunc[i][j].input == 0)
                { // 输入是0的情况
                    in0 *= 10;
                    in0 += transFunc[i][j].output;
                    s0 = s0 + to_string(transFunc[i][j].output);
                }
                else if (transFunc[i][j].input == 1)
                { // 输入是1的情况
                    in1 *= 10;
                    in1 += transFunc[i][j].output;
                    s1 = s1 + to_string(transFunc[i][j].output);
                }
            }

            transFunc[i].clear();
            if (f0)
            {
                transFunc[i].push_back(FANode(0, in0));
            }
            if (f0)
            {
                //myTransFunc[i].push_back(Node(0, s0));
            }

            if (f1)
            {
                transFunc[i].push_back(FANode(1, in1));
            }
            if (f1)
            {
                //myTransFunc[i].push_back(Node(1, s1));
            }
        }
    }

    void convert()
    {
        map<int, int> map;
        for (int i = 0; i < Status.size(); i++)
        {
            map.insert(pair<int, int>(Status[i], i));
        }

        for (int i = 0; i < Status.size(); i++)
        {
            Status[i] = i;
        }

        for (int i = 0; i < transFunc.size(); i++)
        {
            for (int j = 0; j < transFunc[i].size(); j++)
            {
                transFunc[i][j].output = map.find(transFunc[i][j].output)->second;
            }
        }

        for (int i = 0; i < terminalStatus.size(); i++)
        {
            terminalStatus[i] = map.find(terminalStatus[i])->second;
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
            cout << i << ": ";
            if (this->transFunc[i].empty())
            {
                cout << "no transfer function!";
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
