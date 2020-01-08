#include "test_runner.h"

#include <vector>
#include <algorithm>

using namespace std;

template <typename T>
void RemoveDuplicates(vector<T>& elements) {
    sort(begin(elements), end(elements));
    elements.erase(unique(begin(elements), end(elements)), end(elements));
}

void Test() {
    vector<int> v1 = { 6, 4, 7, 6, 4, 4, 0, 1 };
    RemoveDuplicates(v1);
    AssertEqual(v1.size(), 5);

    vector<string> v2 = {"C", "C++", "C++", "C", "C++"};
    RemoveDuplicates(v2);
    AssertEqual(v2.size(), 2);
}

int main() {
    TestRunner runner;
    runner.RunTest(Test, "Test");
    return 0;
}
