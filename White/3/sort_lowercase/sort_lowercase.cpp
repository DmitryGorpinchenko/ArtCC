#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctype.h>

using namespace std;

int main() {
    int n;
    cin >> n;
    vector<string> strings(n);
    for (auto& s : strings) {
        cin >> s;
    }
    sort(begin(strings), end(strings), [](const string& l, const string& r) {
        return lexicographical_compare(begin(l), end(l), begin(r), end(r), [](char c1, char c2) {
            return tolower(c1) < tolower(c2);
        });
    });
    for (auto s : strings) {
        cout << s << " ";
    }
    cout << endl;
    return 0;
}
