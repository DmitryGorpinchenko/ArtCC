#include <iostream>
#include <vector>

std::vector<bool> binaryDigits(int n) {
    std::vector<bool> d;
    while (n > 0) {
        d.push_back(n & 1);
        n >>= 1;
    }
    return d;
}

int main() {
    int n;
    std::cin >> n;
    const auto digits = binaryDigits(n);
    for (int i = digits.size() - 1; i >= 0; --i) {
        std::cout << digits[i];
    }
    std::cout << std::endl;
    return 0;
}
