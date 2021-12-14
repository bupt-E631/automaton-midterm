#include"output.h"

void output(MinimizedDFA* mdfa, bool whetherOutPutRL) {
    set<int> endStatus;
    vector <map<int, int>> transFunction;
    for (auto x : mdfa->terminalStatus)
        endStatus.insert(x);
    for (int i = 0; i < mdfa->numOfStatus; i++) {
        map<int, int> temp;
        for (auto& x : mdfa->transFunc[i])
            temp.insert({ x.input,x.output });
        transFunction.push_back(temp);
    }
    cout << '\t' << 0 << '\t' << 1 << endl;
    for (int i = 0; i < mdfa->numOfStatus; i++) {
        if (i == mdfa->beginStatus)
            cout << '#';
        else
            cout << ' ';
        if (endStatus.count(i))
            cout << '*';
        else
            cout << ' ';
        cout << 'q' << i + 1 << "   ";
        try {
            int temp = transFunction[i].at(0);
            cout << 'q' << temp + 1 << "\t";
        }
        catch (out_of_range) {
            cout << "N\t";
        }
        try {
            int temp = transFunction[i].at(1);
            cout << 'q' << temp + 1 << "\t";
        }
        catch (out_of_range) {
            cout << "N\t";
        }
        cout << endl;
    }
}

void output_file(MinimizedDFA* mdfa, bool whetherOutPutRL) {
    fs::path p{ "ans.txt" };
    ofstream output{ p };
    set<int> endStatus;
    vector <map<int, int>> transFunction;
    for (auto x : mdfa->terminalStatus)
        endStatus.insert(x);
    for (int i = 0; i < mdfa->numOfStatus; i++) {
        map<int, int> temp;
        for (auto& x : mdfa->transFunc[i])
            temp.insert({ x.input,x.output });
        transFunction.push_back(temp);
    }
    output << ' ' << 0 << ' ' << 1 << '\n';
    for (int i = 0; i < mdfa->numOfStatus; i++) {
        if (i == mdfa->beginStatus)
            output << '#';
        /*else
            output << ' ';*/
        if (endStatus.count(i))
            output << '*';
        /*else
            output << ' ';*/
        output << 'q' << i + 1 << ' ';
        try {
            int temp = transFunction[i].at(0);
            output << 'q' << temp + 1 << ' ';
        }
        catch (out_of_range) {
            output << "N ";
        }
        try {
            int temp = transFunction[i].at(1);
            output << 'q' << temp + 1 << ' ';
        }
        catch (out_of_range) {
            output << "N ";
        }
        output << '\n';
    }
}