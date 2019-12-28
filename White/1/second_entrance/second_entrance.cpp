#include <iostream>
#include <string>

int find(char c, const std::string& s, int from) {
    for (int i = from; i < s.size(); ++i) {
        if (s[i] == c) {
            return i;
        }
    }
    return -1;
}

int main() {
    std::string s;
    std::cin >> s;
    auto c = 'f';
    auto id = find(c, s, 0);
    if (id < 0) {
        std::cout << -2;
    } else {
        id = find(c, s, id + 1);
        if (id < 0) {
            std::cout << -1;
        } else {
            std::cout << id;
        }
    }
    std::cout << std::endl;
    return 0;
}
