#include <iostream>
#include <vector>

int Average(const std::vector<int>& v) {
    int res = 0;
    for (auto n : v) {
        res += n;
    }
    return res / v.size();
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

int main() {
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
