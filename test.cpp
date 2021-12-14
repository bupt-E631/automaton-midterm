#include"output.h"

int main() {
    cout << "please enter a RE to test: ";
    string s;
    cin >> s;
    RegularExpression re{ s };
    epsilon_NFA en = re.to_epsilon_NFA();
    NFA* nfa = en.toNFA();
    DFA* dfa = nfa->toDFA(true);
    MinimizedDFA* mdfa = dfa->toMinimizedDFA();
    output(mdfa, 0);
    output_file(mdfa, 0);
    getchar();
    return 0;
}