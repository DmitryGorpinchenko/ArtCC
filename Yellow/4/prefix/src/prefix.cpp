#include "test_runner.h"

#include <utility>
#include <string>
#include <algorithm>

using namespace std;

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, char prefix) {
    auto l = lower_bound(range_begin, range_end, string(1, prefix));
    auto r = lower_bound(l, range_end, string(1, char(prefix + 1)));
    return { l, r };
}

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(RandomIt range_begin, RandomIt range_end, const string& prefix) {
    auto next = prefix;
    ++next.back();
    auto l = lower_bound(range_begin, range_end, prefix);
    auto r = lower_bound(l, range_end, next);
    return { l, r };
}

void TestCharPrefix() {
    {
        const vector<string> empty;
        auto res = FindStartsWith(begin(empty), end(empty), 'm');
        Assert((res.first == res.second) && (res.first == end(empty)), "range is not empty");
    }
    {
        const vector<string> strings = { "moscow", "murmansk", "vologda" };
        
        auto res = FindStartsWith(begin(strings), end(strings), 'm');
        AssertEqual(vector<string>{ res.first, res.second }, vector<string>{ "moscow", "murmansk" });
        
        res = FindStartsWith(begin(strings), end(strings), 'v');
        AssertEqual(vector<string>{ res.first, res.second }, vector<string>{ "vologda" });
        
        res = FindStartsWith(begin(strings), end(strings), 'p');
        Assert(res.first == res.second, "range is not empty");
        AssertEqual(res.first - begin(strings), 2);
        
        res = FindStartsWith(begin(strings), end(strings), 'z');
        Assert(res.first == res.second, "range is not empty");
        AssertEqual(res.first - begin(strings), 3);
    }
}

void TestStrPrefix() {
    {
        const vector<string> empty;
        auto res = FindStartsWith(begin(empty), end(empty), "mmm");
        Assert((res.first == res.second) && (res.first == end(empty)), "range is not empty");
    }
    {
        const vector<string> strings = { "moscow", "motovilikha", "murmansk" };

        auto res = FindStartsWith(begin(strings), end(strings), "mo");
        AssertEqual(vector<string>{ res.first, res.second }, vector<string>{ "moscow", "motovilikha" });
        
        res = FindStartsWith(begin(strings), end(strings), "mt");
        Assert(res.first == res.second, "range is not empty");
        AssertEqual(res.first - begin(strings), 2);

        res = FindStartsWith(begin(strings), end(strings), "na");
        Assert(res.first == res.second, "range is not empty");
        AssertEqual(res.first - begin(strings), 3);
    }
}

int main() {
    TestRunner runner;
    runner.RunTest(TestCharPrefix, "TestCharPrefix");
    runner.RunTest(TestStrPrefix, "TestStrPrefix");
    return 0;
}
