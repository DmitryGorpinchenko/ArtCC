#include <iostream>
#include <unordered_map>

std::unordered_map<char, int> BuildCharCounters(const std::string& s) {
    std::unordered_map<char, int> res;
    for (auto c : s) {
        ++res[c];
    }
    return res;
}

bool IsAnagrams(const std::string& l, const std::string& r) {
    return BuildCharCounters(l) == BuildCharCounters(r);
}

int main() {
    int n;
    std::cin >> n;
    std::string l, r;
    for (int i = 0; i < n; ++i) {
        std::cin >> l >> r;
        std::cout << (IsAnagrams(l, r) ? "YES" : "NO") << std::endl;
    }
    return 0;
}
