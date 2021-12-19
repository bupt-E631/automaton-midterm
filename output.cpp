#include"output.h"

void output(MinimizedDFA* mdfa) {
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

void output_file(MinimizedDFA* mdfa) {
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
    output << '\t' << 0 << '\t' << 1 << '\n';
    for (int i = 0; i < mdfa->numOfStatus; i++) {
        if (i == mdfa->beginStatus)
            output << '#';
        else
            output << ' ';
        if (endStatus.count(i))
            output << '*';
        else
            output << ' ';
        output << 'q' << i + 1 << '\t';
        try {
            int temp = transFunction[i].at(0);
            output << 'q' << temp + 1 << '\t';
        }
        catch (out_of_range) {
            output << "N "<<'\t';
        }
        try {
            int temp = transFunction[i].at(1);
            output << 'q' << temp + 1 << '\t';
        }
        catch (out_of_range) {
            output << "N ";
        }
        output << '\n';
    }
    output.close();
    output.open("re.txt");
    for (int i = 0; i < mdfa->numOfStatus; i++) {
        output << static_cast<char>(i + 65) << " > ";
        try {
            int temp = transFunction[i].at(0);
            output << 0 << static_cast<char>(temp + 65) << '|';
        }
        catch (out_of_range) {
            output << 0 << '|';
        }
        try {
            int temp = transFunction[i].at(1);
            output << 1 <<static_cast<char>(temp + 65) ;
        }
        catch (out_of_range) {
            output << 1 ;
        }
        output << endl;
    }
    output.close();
    output.open("graphviz.txt");
    output << "start" << " [shape=circle style=filled fillcolor=red]\n";
    for (int i = 0; i < mdfa->numOfStatus; i++) {
        if (endStatus.count(i))
            output << 'q' << i + 1 << " [shape=doublecircle]\n";
        else
            output << 'q' << i + 1 << " [shape=circle]\n";
    }
    output << "start" << " -> " << 'q' << mdfa->beginStatus + 1 << '\n';
    for (int i = 0; i < mdfa->numOfStatus; i++) {
        try {
            int temp = transFunction[i].at(0);
            output << 'q' << i + 1 << " -> " << 'q' << temp + 1 << " [label=0]\n";
        }
        catch (out_of_range) {  }
        try {
            int temp = transFunction[i].at(1);
            output << 'q' << i + 1 << " -> " << 'q' << temp + 1 << " [label=1]\n";
        }
        catch (out_of_range) {  }
    }
    output.close();
}