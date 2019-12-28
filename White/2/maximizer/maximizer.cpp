#include <iostream>

void UpdateIfGreater(int first, int& second) {
    if (first > second) {
        second = first;
    }
}

int main() {
    int a = 4;
    int b = 2;
    UpdateIfGreater(a, b);
    std::cout << b << std::endl;
    return 0;
}
