#include"NFA.h"

NFA::NFA() = default;

DFA* NFA::toDFA(bool isThis)
{
    transFuncConvert(); //垃圾架构转换为我的架构
    statusMapping();    //垃圾架构状态向量转换为我的向量
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

void NFA::toTerminal()
{
    //清空原结束状态表
    terminalStatus.clear();
    for (int i = 0; i < myTer.size(); i++)
    {
        //用我的结束状态表对照myStatus找位置存入
        for (int j = 0; j < Status.size(); j++)
        { //状态位置
            if (isSameSet(myTer[i], myStatus[j]))
            {
                terminalStatus.push_back(j);
            }
        }
    }
}

void NFA::toTransFunc()
{
    transFunc.clear();
    //表先建起来
    for (int i = 0; i < myTransFunc.size(); i++)
    {
        transFunc.push_back(vector<FANode>());
    }

    //修改数据
    for (int i = 0; i < myTransFunc.size(); i++)
    {
        //输入0
        if (myTransFunc[i][0].size() == 0)
        {
            //空集合,说明不接受,直接跳过
        }
        else
        { //接收
            for (int j = 0; j < Status.size(); j++)
            {
                if (isSameSet(myTransFunc[i][0], myStatus[j]))
                {
                    transFunc[i].push_back(FANode(0, j));
                }
            }
        }
        //输入1
        if (myTransFunc[i][1].size() != 0)
        {
            for (int j = 0; j < Status.size(); j++)
            {
                if (isSameSet(myTransFunc[i][1], myStatus[j]))
                {
                    transFunc[i].push_back(FANode(1, j));
                }
            }
        }
    }
}

bool NFA::isSameSet(set<int> s0, set<int> s1)
{
    if (s0.size() != s1.size())
    {
        return false;
    }
    else
    {
        set<int> temp;
        set_symmetric_difference(s0.begin(), s0.end(), s1.begin(), s1.end(), inserter(temp, temp.begin()));
        return temp.size() == 0;
    }
}

void NFA::toStatus()
{
    for (int i = Status.size(); i < myStatus.size(); i++)
    {
        //根据我的映射表将Status修改过来
        Status.push_back(i);
    }
}

void NFA::addTer()
{
    for (int i = 0; i < myStatus.size(); i++)
    {
        for (int j = 0; j < terminalStatus.size(); j++)
        { //如果包含原生结束状态,说明此状态是结束状态
            if (myStatus[i].count(terminalStatus[j]) != 0)
            {
                myTer.push_back(myStatus[i]);
            }
        }
    }
}

void NFA::formExtend(set<int> unit)
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

void NFA::myDisplay()
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

bool NFA::exist(set<int> unit)
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

void NFA::statusMapping()
{
    for (int i = 0; i < Status.size(); i++)
    {
        set<int> temp;
        temp.insert(Status[i]);
        myStatus.push_back(temp);
    }
}

void NFA::transFuncConvert()
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