#include <bits/stdc++.h>
using namespace std;

struct State {
    int b5, b3;
};

const int CAP5 = 5, CAP3 = 3;

void printState(const State &e, int level, const string &action) {
    // tab
    for (int i = 0; i < level; i++){
        cout << "|   ";
    }
    // bucket (x, y)
    cout << "(" << e.b5 << "," << e.b3 << ")";
    // -- action
    if (!action.empty()){
        cout << " -- " << action;
    }
    cout << "\n";
}

void generateTree(const State &e, int level, set<pair<int,int>> visit) {
    visit.insert({e.b5, e.b3});

    // verify if end
    if (e.b5 + e.b3 == 7) {
        // print the final state and donot generate more node from here
        std:: string message = "Final state - Reached with ";
        message = message.append(std::to_string(level));
        printState(e, level, message);
        return;
    }

    // The possibilities - what the program gona do
    vector<pair<State,string>> next;

    next.push_back({{CAP5, e.b3}, "Fill bucket of 5"});
    next.push_back({{e.b5, CAP3}, "Fill bucket of 3"});
    next.push_back({{0, e.b3}, "Empty bucket of 5"});
    next.push_back({{e.b5, 0}, "Empty bucket of 3"});

    int transf = min(e.b5, CAP3 - e.b3);
    next.push_back({{e.b5 - transf, e.b3 + transf}, "Transfer from 5 to 3"});

    transf = min(e.b3, CAP5 - e.b5);
    next.push_back({{e.b5 + transf, e.b3 - transf}, "Transfer from 3 to 5"});

    // recursive call next nodes
    for (auto &prox : next) {
        //verify if it the state is new. and never visited.
        if (!visit.count({prox.first.b5, prox.first.b3})) {
            printState(prox.first, level + 1, prox.second);
            generateTree(prox.first, level + 1, visit);
        }
    }
}

int main() {
    State initial = {0, 0};
    printState(initial, 0, "Start");
    generateTree(initial, 0, {});
    return 0;
}