#include"output.h"

int main() {
    fs::path p{ "re_1.txt" };
    ifstream input{ p };
    if (input.fail() == true){
        cout << "�ļ�re_1.txtû�гɹ��򿪣��밴������˳�..." << endl;
        getchar();
        return 0;
    }
    cout << "���ڴ�re_1.txt�ж�ȡ������ʽ�����Ժ�..." << endl;
    string s;
    input >> s;
    //���re->��С��DFA��ת��
    RegularExpression re{ s };
    epsilon_NFA en = re.to_epsilon_NFA();
    NFA* nfa = en.toNFA();
    DFA* dfa = nfa->toDFA(true);
    MinimizedDFA* mdfa = dfa->toMinimizedDFA();
    //������ض����ļ�
    output_file(mdfa);
    //system("to_pic.bat");
    system("echo strict digraph res{ > res.dot");
    system("type graphviz.txt >> res.dot");
    system("echo } >> res.dot");
    system("dot -Tpng res.dot -o output_result/dfa.png");
    system("del res.dot");
    system("del graphviz.txt");
    cout << "��������ɣ��밴������˳�..." << endl;
    getchar();
    return 0;
}