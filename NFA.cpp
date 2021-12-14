#include"NFA.h"

NFA::NFA() = default;

DFA* NFA::toDFA(bool isThis)
{
    transFuncConvert(); //�����ܹ�ת��Ϊ�ҵļܹ�
    statusMapping();    //�����ܹ�״̬����ת��Ϊ�ҵ�����
    //�����ҵı�ȥ��ת��
    for (int i = 0; i < myTransFunc.size(); i++)
    {
        for (int j = 0; j < myTransFunc[i].size(); j++)
        {
            formExtend(myTransFunc[i][j]);
        }
    }

    //��Terminal
    addTer();

    //myStatus��ת���Ĺؼ�

    //���ҵ�״̬ӳ�佫�����ܹ���ӳ�����
    toStatus();

    //�����ҵ�ӳ��״̬��ȥת��Ϊ������
    toTransFunc();

    //���ҵĽ���״̬ת��Ϊ��������״̬
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
    //���ԭ����״̬��
    terminalStatus.clear();
    for (int i = 0; i < myTer.size(); i++)
    {
        //���ҵĽ���״̬�����myStatus��λ�ô���
        for (int j = 0; j < Status.size(); j++)
        { //״̬λ��
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
    //���Ƚ�����
    for (int i = 0; i < myTransFunc.size(); i++)
    {
        transFunc.push_back(vector<FANode>());
    }

    //�޸�����
    for (int i = 0; i < myTransFunc.size(); i++)
    {
        //����0
        if (myTransFunc[i][0].size() == 0)
        {
            //�ռ���,˵��������,ֱ������
        }
        else
        { //����
            for (int j = 0; j < Status.size(); j++)
            {
                if (isSameSet(myTransFunc[i][0], myStatus[j]))
                {
                    transFunc[i].push_back(FANode(0, j));
                }
            }
        }
        //����1
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
        //�����ҵ�ӳ���Status�޸Ĺ���
        Status.push_back(i);
    }
}

void NFA::addTer()
{
    for (int i = 0; i < myStatus.size(); i++)
    {
        for (int j = 0; j < terminalStatus.size(); j++)
        { //�������ԭ������״̬,˵����״̬�ǽ���״̬
            if (myStatus[i].count(terminalStatus[j]) != 0)
            {
                myTer.push_back(myStatus[i]);
            }
        }
    }
}

void NFA::formExtend(set<int> unit)
{
    //���ж����״̬�Ƿ�Ϊ��
    if (unit.size() == 0)
    {
        return;
    }

    //�жϸ�״̬�Ƿ������״̬��
    if (exist(unit))
    {
        return;
    }

    // �ü��ϲ�������״̬��,���¼���
    //����״̬��
    myStatus.push_back(unit);
    //���д������� --���һ�д����״̬
    vector<set<int>> temp;
    temp.push_back(set<int>());
    temp.push_back(set<int>());
    myTransFunc.push_back(temp);
    numOfStatus++;
    //ȷ�����״̬ --��״̬ʵ��ԭ��״̬�Ļ����Ϻϲ�������,���Ը���Ԫ�ؿ϶���ԭԭ��״̬
    set<int> buffer0;
    set<int> buffer1;
    for (set<int>::iterator iter = unit.begin(); iter != unit.end(); iter++)
    {
        //����0
        set<int> temp0;
        //myTransFunc[*iter][0]; --Ϊ��Ӧ����
        set_union(myTransFunc[*iter][0].begin(), myTransFunc[*iter][0].end(), buffer0.begin(), buffer0.end(), inserter(temp0, temp0.begin()));
        buffer0.clear();
        buffer0 = temp0;
        //����1
        set<int> temp1;
        //myTransFunc[*iter][1];
        set_union(myTransFunc[*iter][1].begin(), myTransFunc[*iter][1].end(), buffer1.begin(), buffer1.end(), inserter(temp1, temp1.begin()));
        buffer1.clear();
        buffer1 = temp1;
    }
    //�����Ӧ��
    myTransFunc[myTransFunc.size() - 1][0] = buffer0;
    myTransFunc[myTransFunc.size() - 1][1] = buffer1;
}

void NFA::myDisplay()
{
    for (int i = 0; i < myStatus.size(); i++)
    {
        //���ͷ
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
        { //��С��ͬ�϶�����һ��״̬
            continue;
        }
        else
        { // ���ϸ�����ȵ�������ж��Ƿ���ͬһ������
            //���ݶԳƲ�Ƿ�Ϊ���жϼ����Ƿ����
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
    //1.�ȹ����ձ�
    vector<set<int>> temp;
    temp.push_back(set<int>());
    temp.push_back(set<int>());
    for (int i = 0; i < numOfStatus; i++)
    {
        myTransFunc.push_back(temp);
    }
    //2.�����ݷ���ձ�
    for (int i = 0; i < transFunc.size(); i++)
    {
        for (int j = 0; j < transFunc[i].size(); j++)
        {
            if (transFunc[i][j].input == 0)
            {
                myTransFunc[i][0].insert(transFunc[i][j].output); //������1ת����ȥ��״̬�����Ӧλ��
            }
            else if (transFunc[i][j].input == 1)
            {
                myTransFunc[i][1].insert(transFunc[i][j].output); //������1ת����ȥ��״̬�����Ӧλ��
            }
        }
    }
}