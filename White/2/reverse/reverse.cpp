#include <iostream>
#include <vector>

void Swap(int& a, int& b) {
    const auto tmp = a;
    a = b;
    b = tmp;
}

void Reverse(std::vector<int>& v) {
    for (decltype(v.size()) i = 0, count = v.size(); i < count/2; ++i) {
        const auto ii = count - i - 1;
        Swap(v[i], v[ii]);
    }
}

std::vector<int> Reversed(const std::vector<int>& v) {
    return { std::rbegin(v), std::rend(v) };
}

int main() {
    std::vector<int> numbers = { 1, 5, 3, 4, 2 };
    for (auto n : Reversed(numbers)) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    return 0;
}
