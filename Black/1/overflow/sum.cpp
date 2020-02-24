#include <iostream>
#include <cstdint>
#include <limits>
#include <optional>

std::optional<int64_t> Sum(int64_t a, int64_t b) {
    if (a > 0 && b > 0) {
        int64_t max = std::numeric_limits<int64_t>::max();
        if (a > max - b) {
            return std::nullopt;
        }
    } else if (a < 0 && b < 0) {
        int64_t min = std::numeric_limits<int64_t>::min();
        if (a < min - b) {
            return std::nullopt;
        }
    }
    return a + b;
}

int main() {
    int64_t a, b;
    std::cin >> a >> b;
    
    if (auto s = Sum(a, b)) {
        std::cout << *s << '\n';
    } else {
        std::cout << "Overflow!\n";
    }

    return 0;
}
