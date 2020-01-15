#include <iostream>
#include <vector>

using namespace std;

static const int MAX_NUMBER = 100'000;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<bool> has(MAX_NUMBER + 1, false);
    vector<int> prev(MAX_NUMBER + 1, -1);
    int end_ = -1;
    for (int i = 0; i < n; ++i) {
        int curr, next;
        cin >> curr >> next;
        if (has[next]) {
            prev[curr] = prev[next];
            prev[next] = curr;
        } else {
            prev[curr] = end_;
            end_ = curr;
        }
        has[curr] = true;
    }
    vector<int> sportsmens;
    sportsmens.reserve(n);
    int curr = end_;
    while (curr >= 0) {
        sportsmens.push_back(curr);
        curr = prev[curr];
    }
    for (auto it = rbegin(sportsmens); it != rend(sportsmens); ++it) {
        cout << *it << ' ';
    }
    cout << '\n';
    return 0;
}
