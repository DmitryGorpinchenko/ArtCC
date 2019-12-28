#include <iostream>
#include <fstream>
#include <iomanip>

void PrintTable(std::ostream& out, const std::string& csv_file) {
    std::ifstream in(csv_file);
    if (!in) {
        return;
    }
    int n = 0, m = 0;
    in >> n >> m;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int el = 0;
            in >> el;
            out << std::setw(10) << el;
            if (j < m - 1) {
                in.ignore(1);
                out << ' ';
            }
        }
        if (i < n - 1) {
            out << std::endl;
        }
    }
}

int main() {
    PrintTable(std::cout, "input.txt");
    return 0;
}
