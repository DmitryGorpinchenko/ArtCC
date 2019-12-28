#include <iostream>
#include <vector>
#include <string>

void MoveStrings(std::vector<std::string>& src, std::vector<std::string>& dst) {
    for (const auto& s : src) {
        dst.push_back(s);
    }
    src.clear();
}

int main() {
    std::vector<std::string> src = { "a", "b", "c" };
    std::vector<std::string> dst = { "z" };
    MoveStrings(src, dst);
    std::cout << src.size() << std::endl;
    for (const auto& s : dst) {
        std::cout << s << " ";
    }
    std::cout << std::endl;
    return 0;
}
