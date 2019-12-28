#include <iostream>
#include <string>

bool IsPalindrom(const std::string& s) {
    for (decltype(s.size()) i = 0, count = s.size(); i < count/2; ++i) {
        const auto ii = count - i - 1;
        if (s[i] != s[ii]) {
            return false;
        }
    }
    return true;
}

int main() {
    std::string s;
    std::cin >> s;
    std::cout << IsPalindrom(s) << std::endl;
    return 0;
}
