#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <ctype.h>

bool LowerCaseLess(const std::string& s1, const std::string& s2) {
    const auto op = [](char c) { return tolower(c); };
    std::string lower_s1, lower_s2;
    lower_s1.reserve(s1.size());
    lower_s2.reserve(s2.size());
    std::transform(std::begin(s1), std::end(s1), std::back_inserter(lower_s1), op);
    std::transform(std::begin(s2), std::end(s2), std::back_inserter(lower_s2), op);
    return (lower_s1 == lower_s2) ? (s1 < s2) : (lower_s1 < lower_s2);
}

int main() {
    int n;
    std::cin >> n;
    std::vector<std::string> strings;
    strings.reserve(n);
    for (int i = 0; i < n; ++i) {
        std::string s;
        std::cin >> s;
        strings.push_back(s);
    }
    std::sort(std::begin(strings), std::end(strings), LowerCaseLess);
    for (auto s : strings) {
        std::cout << s << " ";
    }
    std::cout << std::endl;
    return 0;
}
