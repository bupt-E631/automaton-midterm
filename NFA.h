/*
 * @Author: your name
 * @Date: 2021-12-05 12:20:54
 * @LastEditTime: 2021-12-12 21:37:25
 * @LastEditors: Karigen B
 * @Description: 完成NFA到DFA的转换
 * @FilePath: \automaton-midterm\NFA.h
 */

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include<set>
#include<map>
#include "FANode.h"
#include "DFA.h"
using namespace std;
class NFA
{
public:
    //状态集合，转移函数，开始状态，结束状态集合
    //所有状态用数字命名

    //Variable:
    int numOfStatus; // 状态数量
    int beginStatus; // 起始状态
    vector<int> Status; // 状态
    vector<vector<FANode>> transFunc; // 表
    vector<vector<FANode>> transFuncBak;
    vector<int> terminalStatus; // 结束状态
    vector<int> terminalStatusBak; // 结束状态
    NFA(){};
    //Function:
    // 

    DFA* toDFA(){
        transFuncBak = transFunc;
        terminalStatusBak = terminalStatus;
        //transFunc.clear();
        FANodeConvert();
        // NFA -> DFA
        for (int i = 0; i < transFunc.size(); i++) { // 外层循环,状态数
            for (int j = 0; j < transFunc[i].size(); j++) { // 内层循环,0/1的输入状态方程
                FANodeExtend(transFunc[i][j]);
            }
        }

        convert();
        
        // 返回
        
        DFA* dfa = new DFA();
        dfa->beginStatus = this->beginStatus;
        dfa->numOfStatus = this->numOfStatus;
        dfa->Status = this->Status;
        dfa->terminalStatus = this->terminalStatus;
        dfa->transFunc = this->transFunc;
        return dfa;
    }

    void FANodeExtend(FANode node){
        //
        for (int i = 0; i < Status.size(); i++) {
            if(Status[i] == node.output){
                return;
            }
        }
        
        //构建新的一行
        Status.push_back(node.output);//放入状态表
        numOfStatus++;
        transFunc.push_back(vector<FANode>());

        //处理新状态
        int count = to_string(node.output).length();//需要比较的次数
        int is=0;//要分析的状态
        for (int k = 0; k <= 1; k++) {//输入0或1
            is = node.output;
            set<int, greater<int>> SSet;
            for (int i = 0; i < count; i++) {
                int tis = is % 10;
                for (int j = 0; j < transFuncBak[tis].size(); j++) {
                    if(transFuncBak[tis][j].input == k){ // 0或1
                        SSet.insert(transFuncBak[tis][j].output);
                    }
                }
            
                is /= 10;
            }

            if(SSet.size()==0){
                continue;
            }

            bool isTer = false;
            for (int i = 0; i < terminalStatusBak.size() && !isTer; i++) {
                if(SSet.count(terminalStatusBak[i])){
                    isTer = true;
                }
            }
            
            if(isTer){
                terminalStatus.push_back(node.output);
            }

            //生成新状态的输出状态
            int rs = 0;//新状态
            for (set<int>::iterator iter = SSet.begin(); iter != SSet.end(); iter++) {
                rs *= 10;
                rs += *iter;
            }
            transFunc[transFunc.size()-1].push_back(FANode(k, rs));
        }
    }

    void FANodeConvert(){
        for (int i = 0; i < transFunc.size(); i++){
            bool f0 = false;
            bool f1 = false;
            for (int j = 0; j < transFunc[i].size(); j++) {
                if (transFunc[i][j].input == 0) {
                    f0 = true;
                } else if (transFunc[i][j].input == 1) {
                    f1 = true;
                }
            }

            int in0 = 0;
            int in1 = 0;
            for (int j = transFunc[i].size(); j >= 0 ; j--){ // 整数最高位不能是0
                if (transFunc[i][j].input == 0) { // 输入是0的情况
                    in0 *= 10;
                    in0 += transFunc[i][j].output;
                } else if (transFunc[i][j].input == 1){ // 输入是1的情况
                    in1 *= 10;
                    in1 += transFunc[i][j].output;
                }
            }

            transFunc[i].clear();
            if(f0){
                transFunc[i].push_back(FANode(0, in0));
            }

            if(f1){
                transFunc[i].push_back(FANode(1, in1));
            }
        }
    }

    void convert(){
        map<int, int> map;
        for (int i = 0; i < Status.size(); i++) {
            map.insert(pair<int, int>(Status[i], i));
        }
        
        for (int i = 0; i < Status.size(); i++) {
            Status[i] = i;
        }
        
        for (int i = 0; i < transFunc.size(); i++) {
            for (int j = 0; j < transFunc[i].size(); j++) {
                transFunc[i][j].output = map.find(transFunc[i][j].output)->second;
            }
        }

        for (int i = 0; i < terminalStatus.size(); i++) {
            terminalStatus[i] = map.find(terminalStatus[i])->second;
        }
        
    }

    void display()
    {
        for (int i = 0; i < this->transFunc.size(); i++)
        {
            cout << Status[i] << ": ";
            for (int j = 0; j < this->transFunc[i].size(); j++)
            {
                cout << "(" << this->transFunc[i][j].input << "," << this->transFunc[i][j].output << ") ";
            }
            cout << endl;
        }
    }
};