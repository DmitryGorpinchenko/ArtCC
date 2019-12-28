#include <iostream>
#include <fstream>

void Copy(const std::string& from, const std::string& to) {
    std::ifstream in(from);
    std::ofstream out(to);
    if (in && out) {
        std::string line;
        while (getline(in, line)) {
            out << line << std::endl;
        }
    }
}

int main() {
    Copy("input.txt", "output.txt");
    return 0;
}
