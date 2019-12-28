#include <iostream>
#include <string>
#include <vector>

bool IsPalindrom(const std::string& s) {
    for (decltype(s.size()) i = 0, count = s.size(); i < count/2; ++i) {
        const auto ii = count - i - 1;
        if (s[i] != s[ii]) {
            return false;
        }
    }
    return true;
}

std::vector<std::string> PalindromFilter(const std::vector<std::string>& words, int min_length) {
    std::vector<std::string> res;
    for (const auto& w : words) {
        if (w.size() < min_length) {
            continue;
        }
        if (IsPalindrom(w)) {
            res.push_back(w);
        }
    }
    return res;
}

int main() {
    for (const auto& w : PalindromFilter({ "weew", "bro", "code" }, 4)) {
        std::cout << w << " ";
    }
    std::cout << std::endl;
    return 0;
}
