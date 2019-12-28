#include <iostream>

int gcd(int a, int b) {
    auto max = (b > a) ? b : a;
    auto min = (b > a) ? a : b;
    while (min > 0) {
        const auto mod = max % min;
        max = min;
        min = mod;
    }
    return max;
}

int main() {
    int a, b;
    std::cin >> a >> b;
    std::cout << gcd(a, b) << std::endl;
    return 0;
}
