#include "test_runner.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

vector<string> SplitIntoWords(const string& s) {
    vector<string> res;
    const auto pred = [](char c) { return c != ' '; };
    auto lit = find_if(begin(s), end(s), pred);
    while (lit != end(s)) {
        auto rit = find(lit, end(s), ' ');
        res.push_back({ lit, rit });
        lit = find_if(rit, end(s), pred);
    }
    return res;
}

void Test() {
    {
        AssertEqual(SplitIntoWords("").size(), 0);
        AssertEqual(SplitIntoWords("    ").size(), 0);
    }
    {
        string s = "     C     Cpp     Java  Python     ";
        vector<string> words = SplitIntoWords(s);
        AssertEqual(words.size(), 4);
        AssertEqual(words, vector<string>{ "C", "Cpp", "Java", "Python" });
    }
}

int main() {
    TestRunner runner;
    runner.RunTest(Test, "Test");
    return 0;
}
