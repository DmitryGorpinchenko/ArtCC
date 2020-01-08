#include <iostream>
#include <vector>
#include <cstdint>

int Average(const std::vector<int>& v) {
    int64_t res = 0;
    for (auto n : v) {
        res += n;
    }
    return res / int(v.size());
}

std::vector<int> ThresholdFilter(const std::vector<int>& v, int threshold) {
    std::vector<int> ids;
    for (int i = 0; i < v.size(); ++i) {
        if (v[i] > threshold) {
            ids.push_back(i);
        }
    }
    return ids;
}

void test1() {
    std::vector<int> v(1'000'000, 100'000'000);
    std::cout << "1: " << ((Average(v) == 100'000'000) ? "OK" : "FAIL") << std::endl;
}
void test2() {
    std::vector<int> v(1'000'000, -100'000'000);
    std::cout << "2: " << ((Average(v) == -100'000'000) ? "OK" : "FAIL") << std::endl;
}

int main() {
    //test1();
    //test2();
    int n;
    std::cin >> n;
    std::vector<int> temperatures;
    temperatures.reserve(n);
    for (int i = 0; i < n; ++i) {
        int tmp;
        std::cin >> tmp;
        temperatures.push_back(tmp);
    }
    const auto ids = ThresholdFilter(temperatures, Average(temperatures));
    std::cout << ids.size() << std::endl;
    for (auto i : ids) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return 0;
}
