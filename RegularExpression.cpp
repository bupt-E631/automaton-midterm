#include"RegularExpression.h"

//�����������������ֱ�������+��&��*ʱִ�У����ڶ�ջ����һ��������NFA���кϲ�

//���������̨epsilon_NFA����
epsilon_NFA mergeEN(epsilon_NFA en1, epsilon_NFA en2) {
    epsilon_NFA result;
    result.numOfStatus = 0;
    //����0��״̬��������ͨ����ת��������̨Ҫ�ϲ��Ļ��ӵ���ʼ״̬
    vector<FANode> resultBegin;
    resultBegin.push_back(FANode(-1, 1));
    resultBegin.push_back(FANode(-1, 1 + en1.numOfStatus));
    //��0��״̬����ת�ƺ�����������
    result.Status.push_back(0);
    result.numOfStatus++;
    result.transFunc.push_back(resultBegin);
    //��һ��NFA��״̬�ż�һ��������״̬��
    for (auto& x : en1.Status) {
        x++;
        result.Status.push_back(x);
        result.numOfStatus++;
    }
    //��1��NFA��ת�ƺ����Ľ��״̬�ż�һ��������ת�ƺ�����
    for (auto& x : en1.transFunc) {
        for (auto& y : x)
            y.output += 1;
        result.transFunc.push_back(x);
    }
    //��2��NFA��״̬�ż��ϣ�1+en1.numOfStatus)��������״̬��
    for (auto& x : en2.Status) {
        x += (1 + en1.numOfStatus);
        result.Status.push_back(x);
        result.numOfStatus++;
    }
    //��2�Ż���ת�ƺ����Ľ��״̬�ż��ϣ�1+en1.numOfStatus)��������ת�ƺ�����
    for (auto& x : en2.transFunc) {
        for (auto& y : x)
            y.output += (1 + en1.numOfStatus);
        result.transFunc.push_back(x);
    }
    //��һ����ֹ״̬������״̬��
    result.Status.push_back(1 + en1.numOfStatus + en2.numOfStatus);
    result.numOfStatus++;
    //��ԭ����̨NFA����ֹ״̬ͨ����ת��ָ��������ֹ״̬
    result.transFunc[en1.numOfStatus].push_back(FANode(-1, 1 + en1.numOfStatus + en2.numOfStatus));
    result.transFunc[en1.numOfStatus + en2.numOfStatus].push_back(FANode(-1, 1 + en1.numOfStatus + en2.numOfStatus));
    //������ֹ״̬��ת�ƺ��������յģ�
    vector<FANode> resultEnd;
    result.transFunc.push_back(resultEnd);
    //���������ֹ״̬��Ϊ���һ��״̬
    result.terminalStatus.clear();
    result.terminalStatus.push_back(result.numOfStatus - 1);
    return result;
}

//���������̨epsilon_NFA����
epsilon_NFA combineEN(epsilon_NFA en1, epsilon_NFA en2) {
    //�ý��NFA�ȵ���ԭ1��NFA
    epsilon_NFA result = en1;
    //��en2��״̬����result��
    for (auto& x : en2.Status) {
        //���Ƚ�״̬�ŵ�����
        x += en1.numOfStatus;
        //Ȼ��״̬����result��״̬��
        result.Status.push_back(x);
        //result״̬������һ
        result.numOfStatus++;
    }
    //ʹԭ1��NFA�����һ��״̬��ͨ��һ����ת������2��NFA����ʼ״̬
    result.transFunc[en1.numOfStatus - 1].push_back(FANode(-1, en1.numOfStatus));
    //��2��NFA��ת�ƺ�������result��
    for (auto& x : en2.transFunc) {
        for (auto& y : x)
            //���Ƚ�ת�ƽ����״̬��״̬�ŵ�����
            y.output += en1.numOfStatus;
        //��װ��ת�ƺ�����vector����result
        result.transFunc.push_back(x);
    }
    //������ֹ״̬Ϊԭ2��NFA����ֹ״̬
    result.terminalStatus.clear();
    result.terminalStatus.push_back(result.numOfStatus - 1);
    return result;
}

//�������epsilon_NFA�հ���
epsilon_NFA closureEN(epsilon_NFA en) {
    epsilon_NFA result;
    //���ڱհ�����Ҫ����ʼ�ͽ�β������һ��״̬�������ý����״̬��ΪԭNFA��+2
    result.numOfStatus = en.numOfStatus + 2;
    //��ԭ����״̬�ܾ�����ת���ܵ���ԭ��ʼ״̬
    en.transFunc[en.numOfStatus - 1].push_back(FANode(-1, 0));
    vector<FANode> resultBegin;
    //���µ���ʼ״̬��ת�ƺ����м����ת�Ƶ�1�ź���ֹ״̬�Ľ��
    resultBegin.push_back(FANode(-1, 1));
    resultBegin.push_back(FANode(-1, en.numOfStatus + 1));
    //�����NFA����ʼ״̬����Ϊ0
    result.beginStatus = 0;
    //��״̬0��������״̬��
    result.Status.push_back(0);
    //��0��״̬��ת�ƺ�������result
    result.transFunc.push_back(resultBegin);
    //��ԭNFA��״̬��������״̬��
    for (auto& x : en.Status) {
        //���ȵ���״̬�ţ�����ֻ����ʼ״̬ǰ������һ��״̬��״̬�ż�һ����
        x++;
        //��״̬��������״̬��
        result.Status.push_back(x);
        //���ڿ�ʼ�Ѿ����ú��˽����״̬�����������ﲻ����������һ
    }
    //��ԭ״̬ת�ƺ�����״̬�������󣬽�״̬ת�ƺ�����������
    for (auto& x : en.transFunc) {
        for (auto& y : x)
            //ÿ��ת�ƺ����Ľ��״̬�ż�һ
            y.output++;
        //��ת��״̬��������
        result.transFunc.push_back(x);
    }
    //����һ����ֹ״̬����״̬ת�ƺ���Ϊ��
    result.Status.push_back(result.numOfStatus - 1);
    vector<FANode> resultEnd;
    result.transFunc.push_back(resultEnd);
    //��ԭNFA����ֹ״̬�ܹ�ͨ����ת���ƶ����������ֹ״̬
    result.transFunc[result.numOfStatus - 2].push_back(FANode(-1, result.numOfStatus - 1));
    //���������ֹ״̬��Ϊ���һ��״̬
    result.terminalStatus.clear();
    result.terminalStatus.push_back(result.numOfStatus - 1);
    return result;
}

RegularExpression::RegularExpression(string RE_) {
    this->RE = RE_;
}

string RegularExpression::preProcessing() {
    string temp = "";
    for (int i = 0; i < this->RE.size(); i++)
        if ((this->RE[i] == '0' || this->RE[i] == '1' || this->RE[i] == '*' || this->RE[i] == ')') &&
            (this->RE[i + 1] == '0' || this->RE[i + 1] == '1' || this->RE[i + 1] == '('))
            temp = temp + this->RE[i] + '&';
        else
            temp += this->RE[i];
    return temp;
}

string RegularExpression::toPost() {
    string temp = this->preProcessing();
    string post_RE = "";
    stack<char> charStack;
    for (auto x : temp) {
        if (x == '0' || x == '1')
            post_RE += x;
        else if (x == '(')
            charStack.push(x);
        else if (x == ')')
            while (!charStack.empty()) {
                if (charStack.top() != '(') {
                    post_RE += charStack.top();
                    charStack.pop();
                }
                else {
                    charStack.pop();
                    break;
                }
            }
        else if (x == '*') {
            while (!charStack.empty()) {
                if (charStack.top() == '*') {
                    post_RE += charStack.top();
                    charStack.pop();
                }
                else
                    break;
            }
            charStack.push(x);
        }
        else if (x == '&') {
            while (!charStack.empty()) {
                if (charStack.top() == '*' || charStack.top() == '&') {
                    post_RE += charStack.top();
                    charStack.pop();
                }
                else
                    break;
            }
            charStack.push(x);
        }
        else if (x == '+') {
            while (!charStack.empty()) {
                if (charStack.top() == '*' || charStack.top() == '&' || charStack.top() == '+') {
                    post_RE += charStack.top();
                    charStack.pop();
                }
                else
                    break;
            }
            charStack.push(x);
        }
    }
    while (!charStack.empty()) {
        post_RE += charStack.top();
        charStack.pop();
    }
    return post_RE;
}

epsilon_NFA RegularExpression::to_epsilon_NFA() {
    string post_RE = this->toPost();
    stack<epsilon_NFA> enStack;
    for (auto x : post_RE) {
        //����ǲ��������ͽ���һ���򵥵�epsilon_NFA��ѹջ
        if (x == '0') {
            epsilon_NFA en;
            en.beginStatus = 0;
            en.numOfStatus = 2;
            en.Status.push_back(0);
            en.Status.push_back(1);
            vector<FANode> p;
            en.transFunc.push_back(p);
            en.transFunc.push_back(p);
            en.terminalStatus.push_back(1);
            en.transFunc[0].push_back(FANode(0, 1));
            enStack.push(en);
        }
        else if (x == '1') {
            epsilon_NFA en;
            en.beginStatus = 0;
            en.numOfStatus = 2;
            en.Status.push_back(0);
            en.Status.push_back(1);
            vector<FANode> p;
            en.transFunc.push_back(p);
            en.transFunc.push_back(p);
            en.terminalStatus.push_back(1);
            en.transFunc[0].push_back(FANode(1, 1));
            enStack.push(en);
        }
        //�����+�ţ��ͽ�ջ������epsilon_NFAȡ���������ٽ����ѹ��ջ��
        else if (x == '+') {
            epsilon_NFA en1 = enStack.top();
            enStack.pop();
            epsilon_NFA en2 = enStack.top();
            enStack.pop();
            epsilon_NFA result = mergeEN(en2, en1);
            enStack.push(result);
        }
        //�����&�ţ��ͽ�ջ������epsilon_NFAȡ���������ٽ����ѹ��ջ��
        else if (x == '&') {
            epsilon_NFA en1 = enStack.top();
            enStack.pop();
            epsilon_NFA en2 = enStack.top();
            enStack.pop();
            epsilon_NFA result = combineEN(en2, en1);
            enStack.push(result);
        }
        //�����*�ţ��ͽ�ջ��epsilon_NFAȡ�����հ������ٽ����ѹ��ջ��
        else if (x == '*') {
            epsilon_NFA en1 = enStack.top();
            enStack.pop();
            epsilon_NFA result = closureEN(en1);
            enStack.push(result);
        }
    }
    return enStack.top();
}