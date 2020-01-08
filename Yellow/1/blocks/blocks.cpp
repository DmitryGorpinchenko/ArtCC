#include <iostream>
#include <vector>
#include <cstdint>

struct Block {
    uint16_t w = 0;
    uint16_t h = 0;
    uint16_t d = 0;
    
    uint64_t Vol() const {
        return uint64_t(w) * h * d;
    }
};

std::istream& operator >>(std::istream& in, Block& val) {
    return in >> val.w >> val.h >> val.d;
}

uint64_t Mass(const std::vector<Block>& blocks, unsigned density) {
    uint64_t res = 0;
    for (const auto& b : blocks) {
        res += density * b.Vol();
    }
    return res;
}

void test() {
    std::vector<Block> blocks(100'000, { 10'000, 10'000, 10'000 });
    std::cout << ((Mass(blocks, 100) == 10'000'000'000'000'000'000u) ? "OK" : "FAIL") << std::endl;
}

int main() {
    //test();
    size_t n;
    unsigned density;
    std::cin >> n >> density;
    std::vector<Block> blocks(n);
    for (auto& b : blocks) {
        std::cin >> b;
    }
    std::cout << Mass(blocks, density) << std::endl;
    return 0;
}
