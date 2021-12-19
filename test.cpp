#include"output.h"

int main() {
    fs::path p{ "re_1.txt" };
    ifstream input{ p };
    if (input.fail() == true){
        cout << "File re_1.txt does not open correctly,please press any key to continue..." << endl;
        getchar();
        return 0;
    }
    cout << "Reading from re_1.txt,please waiting..." << endl;
    string s;
    input >> s;
    //完成re->最小化DFA的转化
    RegularExpression re{ s };
    epsilon_NFA en = re.to_epsilon_NFA();
    NFA* nfa = en.toNFA();
    DFA* dfa = nfa->toDFA(true);
    MinimizedDFA* mdfa = dfa->toMinimizedDFA();
    //将输出重定向到文件
    output_file(mdfa);
    //system("to_pic.bat");
    system("echo strict digraph res{ > res.dot");
    system("type graphviz.txt >> res.dot");
    system("echo } >> res.dot");
    system("dot -Tpng res.dot -o output_result/dfa.png");
    system("del res.dot");
    system("del graphviz.txt");
    cout << "Build completed,please press any key to continue..." << endl;
    getchar();
    return 0;
}