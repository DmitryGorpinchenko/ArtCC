#include <iostream>
#include <deque>

using namespace std;

int Precedence(char op) {
    return ((op == '*') || (op == '/') || (op == 0)) ? 1 : 0;
}

int main() {
    char prev_op = 0;
    int x = 0, n = 0;
    cin >> x >> n;
    deque<string> expr;
    expr.push_back(to_string(x));
    for (int i = 0; i < n; ++i) {
        char curr_op;
        cin >> curr_op >> x;
        //const bool parenths_needed = true; // Part1
        const bool parenths_needed = Precedence(prev_op) < Precedence(curr_op); // Part2
        if (parenths_needed) {
            expr.push_front("(");
            expr.push_back(")");
        }
        expr.push_back(" " + string(1, curr_op) + " " + to_string(x));
        prev_op = curr_op;
    }
    for (const auto& s : expr) {
        cout << s;
    }
    cout << endl;
    return 0;
}
