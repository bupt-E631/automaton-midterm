#include"output.h"

void output_file(MinimizedDFA* mdfa) {
    //先重定向输出最小化DFA
    system("mkdir output_result");
    fs::path p{ "output_result\\ans.txt" };
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
    //再重定向输出正则文法
    output.open("output_result\\rg.txt");
    for (int i = 0; i < mdfa->numOfStatus; i++) {
        output << static_cast<char>(i + 65) << " > ";
        try {
            int temp = transFunction[i].at(0);
            output << 0 << static_cast<char>(temp + 65);
            if (endStatus.count(temp))
                output << "|0";
        }
        catch (out_of_range) {    }
        try {
            int temp = transFunction[i].at(1);
            try {
                int temp = transFunction[i].at(0);
                output << '|';
            }
            catch (out_of_range) {}
            output << 1 <<static_cast<char>(temp + 65) ;
            if (endStatus.count(temp))
                output << "|1";
        }
        catch (out_of_range) {    }
        output << endl;
    }
    output.close();
    //最后重定向输出用于绘图的文件
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