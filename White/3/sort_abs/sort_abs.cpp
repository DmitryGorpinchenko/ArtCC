#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int n;
    std::cin >> n;
    std::vector<int> nums;
    nums.reserve(n);
    for (int i = 0; i < n; ++i) {
        int num;
        std::cin >> num;
        nums.push_back(num);
    }
    std::sort(std::begin(nums), std::end(nums), [](int a, int b) { return abs(a) < abs(b); });
    for (auto num : nums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
    return 0;
}
