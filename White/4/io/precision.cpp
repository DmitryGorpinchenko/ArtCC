#include <iostream>
#include <fstream>
#include <iomanip>

int main() {
    std::ifstream in("input.txt");
    std::cout << std::fixed << std::setprecision(3);
    double n;
    while (in >> n) {
        std::cout << n << std::endl;
    }
    return 0;
}
