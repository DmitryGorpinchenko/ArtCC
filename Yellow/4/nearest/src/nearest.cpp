#include "test_runner.h"

#include <set>

using namespace std;

set<int>::const_iterator FindNearestElement(const set<int>& numbers, int border) {
    const auto it = numbers.lower_bound(border);
    if (it == begin(numbers)) {
        return it;
    }
    if (it == end(numbers)) {
        return prev(it);
    }
    const auto d1 = *it - border;
    const auto d2 = border - *prev(it);
    return d1 < d2 ? it : prev(it);
}

void Test() {
    {
        set<int> empty_set;
        Assert(FindNearestElement(empty_set, 0) == end(empty_set), "empty set problem");
    }
    {
        set<int> numbers = { 1, 4, 6 };
        AssertEqual(*FindNearestElement(numbers, 0), 1);
        AssertEqual(*FindNearestElement(numbers, 3), 4);
        AssertEqual(*FindNearestElement(numbers, 4), 4);
        AssertEqual(*FindNearestElement(numbers, 5), 4);
        AssertEqual(*FindNearestElement(numbers, 6), 6);
        AssertEqual(*FindNearestElement(numbers, 100), 6);
    }
}

int main() {
    TestRunner runner;
    runner.RunTest(Test, "Test");
    return 0;
}
