#include "test_runner.h"

#include <algorithm>
#include <vector>
#include <iterator>

using namespace std;

// comment out when submit Part 2
template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if ((range_end - range_begin) > 1) {
        vector<typename RandomIt::value_type> tmp(range_begin, range_end);
        auto l = begin(tmp);
        auto m = l + tmp.size() / 2;
        auto r = end(tmp);
        MergeSort(l, m);
        MergeSort(m, r);
        merge(l, m, m, r, range_begin);
    }
}

// rename to MergeSort when submit Part 2
template <typename RandomIt>
void MergeSort3Way(RandomIt range_begin, RandomIt range_end) {
    const auto len = range_end - range_begin;
    if (len > 1) {
        vector<typename RandomIt::value_type> tmp(range_begin, range_end);
        auto l = begin(tmp);
        auto m1 = l + len / 3;
        auto m2 = l + len * 2 / 3;
        auto r = end(tmp);
        MergeSort3Way(l, m1);
        MergeSort3Way(m1, m2);
        MergeSort3Way(m2, r);
        vector<typename RandomIt::value_type> merge_2_3;
        merge(l, m1, m1, m2, back_inserter(merge_2_3));
        merge(begin(merge_2_3), end(merge_2_3), m2, r, range_begin);
    }
}

void Test2Way() {
    {
        vector<int> v;
        MergeSort(begin(v), end(v));
        AssertEqual(v, vector<int>{});
    }
    {
        vector<int> v = { 7 };
        MergeSort(begin(v), end(v));
        AssertEqual(v, vector<int>{ 7 });
    }
    {
        vector<int> v = { 7, 3 };
        MergeSort(begin(v), end(v));
        AssertEqual(v, vector<int>{ 3, 7 });
    }
    {
        vector<int> v = { 7, 3, 4 };
        MergeSort(begin(v), end(v));
        AssertEqual(v, vector<int>{ 3, 4, 7 });
    }
    {
        vector<int> v = { 7, 3, 4, 1 };
        MergeSort(begin(v), end(v));
        AssertEqual(v, vector<int>{ 1, 3, 4, 7 });
    }
    {
        vector<int> v = { 6, 4, 7, 6, 4, 4, 0, 1 };
        MergeSort(begin(v), end(v));
        AssertEqual(v, vector<int>{ 0, 1, 4, 4, 4, 6, 6, 7 });
    }
    {
        vector<int> v = { 6, 4, 7, 6, 4, 4, 0, 1, 5 };
        MergeSort(begin(v), end(v));
        AssertEqual(v, vector<int>{ 0, 1, 4, 4, 4, 5, 6, 6, 7 });
    }
}

void Test3Way() {
    {
        vector<int> v;
        MergeSort3Way(begin(v), end(v));
        AssertEqual(v, vector<int>{});
    }
    {
        vector<int> v = { 7 };
        MergeSort3Way(begin(v), end(v));
        AssertEqual(v, vector<int>{ 7 });
    }
    {
        vector<int> v = { 7, 3 };
        MergeSort3Way(begin(v), end(v));
        AssertEqual(v, vector<int>{ 3, 7 });
    }
    {
        vector<int> v = { 7, 3, 4 };
        MergeSort3Way(begin(v), end(v));
        AssertEqual(v, vector<int>{ 3, 4, 7 });
    }
    {
        vector<int> v = { 7, 3, 4, 1 };
        MergeSort3Way(begin(v), end(v));
        AssertEqual(v, vector<int>{ 1, 3, 4, 7 });
    }
    {
        vector<int> v = { 6, 4, 7, 6, 4, 4, 0, 1 };
        MergeSort3Way(begin(v), end(v));
        AssertEqual(v, vector<int>{ 0, 1, 4, 4, 4, 6, 6, 7 });
    }
    {
        vector<int> v = { 6, 4, 7, 6, 4, 4, 0, 1, 5 };
        MergeSort3Way(begin(v), end(v));
        AssertEqual(v, vector<int>{ 0, 1, 4, 4, 4, 5, 6, 6, 7 });
    }
}

int main() {
    TestRunner runner;
    runner.RunTest(Test2Way, "Test2Way");
    runner.RunTest(Test3Way, "Test3Way");
    return 0;
}
