#include "test_runner.h"

#include <algorithm>
#include <vector>
#include <iterator>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    const auto len = range_end - range_begin;
    if (len > 1) {
        vector<typename RandomIt::value_type> tmp(make_move_iterator(range_begin),
                                                  make_move_iterator(range_end));
        auto l = begin(tmp);
        auto m1 = l + len / 3;
        auto m2 = l + len * 2 / 3;
        auto r = end(tmp);
        MergeSort(l, m1);
        MergeSort(m1, m2);
        MergeSort(m2, r);
        vector<typename RandomIt::value_type> merge_2_3;
        merge(make_move_iterator(l),
              make_move_iterator(m1),
              make_move_iterator(m1),
              make_move_iterator(m2), back_inserter(merge_2_3));
        merge(make_move_iterator(begin(merge_2_3)),
              make_move_iterator(end(merge_2_3)),
              make_move_iterator(m2),
              make_move_iterator(r), range_begin);
    }
}

void Test3Way() {
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

int main() {
    TestRunner runner;
    runner.RunTest(Test3Way, "Test3Way");
    return 0;
}
