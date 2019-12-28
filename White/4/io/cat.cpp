#include <iostream>
#include <fstream>

void Print(std::ostream& out, const std::string& fname) {
    std::ifstream in(fname);
    std::string line;
    while (getline(in, line)) {
        out << line << std::endl;
    }
}

int main() {
    Print(std::cout, "input.txt");
    return 0;
}
