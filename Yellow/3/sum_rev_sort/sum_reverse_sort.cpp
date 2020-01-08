#include "sum_reverse_sort.h"

#include <algorithm>

int Sum(int x, int y) {
    return x + y;
}

std::string Reverse(std::string s) {
    return { std::rbegin(s), std::rend(s) };
}

void Sort(std::vector<int>& nums) {
    std::sort(std::begin(nums), std::end(nums));
}
