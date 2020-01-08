#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>

void PrintVectorPart(const std::vector<int>& numbers) {
    auto it = std::find_if(std::begin(numbers), std::end(numbers), [](int n) { return n < 0; });
    for (auto rit = std::make_reverse_iterator(it); rit != std::rend(numbers); ++rit) {
        std::cout << *rit << ' ';
    }
}

int main() {
    PrintVectorPart({ 6, 1, 8, -5, 4 });
    std::cout << std::endl;
    PrintVectorPart({ -6, 1, 8, -5, 4 });
    std::cout << std::endl;
    PrintVectorPart({ 6, 1, 8, 5, 4 });
    std::cout << std::endl;
    return 0;
}
