#include <iostream>

double isEven(int n) {
    return (n % 2) == 0;
}

int main() {
    int a, b;
    std::cin >> a >> b;
    for (int i = a; i <= b; ++i) {
        if (isEven(i)) {
            std::cout << i << " ";
        }
    }
    std::cout << std::endl;
    return 0;
}
