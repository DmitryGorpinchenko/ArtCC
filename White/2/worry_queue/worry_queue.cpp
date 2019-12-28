#include <iostream>
#include <vector>
#include <algorithm>

class WorryQueue {
public:
    void come(int k) { q.resize(q.size() + k, false); }
    void worry(int i) { q[i] = true; }
    void quiet(int i) { q[i] = false; }

    int worryCount() const { return std::count(std::begin(q), std::end(q), true); }

private:
    std::vector<bool> q;
};

int main() {
    int n;
    std::cin >> n;
    WorryQueue q;
    for (int i = 0; i < n; ++i) {
        std::string op;
        int param;
        std::cin >> op;
        if (op == "WORRY") {
            std::cin >> param;
            q.worry(param);
        } else if (op == "QUIET") {
            std::cin >> param;
            q.quiet(param);
        } else if (op == "COME") {
            std::cin >> param;
            q.come(param);
        } else if (op == "WORRY_COUNT") {
            std::cout << q.worryCount() << std::endl;
        }
    }
    return 0;
}
