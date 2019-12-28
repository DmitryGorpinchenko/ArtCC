#include <iostream>

int Factorial(int n) {
    int res = 1;
    for (int i = n; i > 1; --i) {
        res *= i;
    }
    return res;
}

int main() {
    int n;
    std::cin >> n;
    std::cout << Factorial(n) << std::endl;
    return 0;
}
