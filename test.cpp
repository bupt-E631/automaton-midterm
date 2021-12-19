#include"output.h"

int main() {
    fs::path p{ "re_1.txt" };
    ifstream input{ p };
    if (input.fail() == true){
        cout << "文件re_1.txt没有成功打开，请按任意键退出..." << endl;
        getchar();
        return 0;
    }
    cout << "正在从re_1.txt中读取正则表达式，请稍候..." << endl;
    string s;
    input >> s;
    RegularExpression re{ s };
    epsilon_NFA en = re.to_epsilon_NFA();
    NFA* nfa = en.toNFA();
    DFA* dfa = nfa->toDFA(true);
    MinimizedDFA* mdfa = dfa->toMinimizedDFA();
    output_file(mdfa);
    cout << "生成已完成，请按任意键退出..." << endl;
    getchar();
    return 0;
}