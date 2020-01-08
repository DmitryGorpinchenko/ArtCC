#include "test_runner.h"

bool IsPalindrom(const std::string& s) {
    for (decltype(s.size()) i = 0, count = s.size(); i < count/2; ++i) {
        const auto ii = count - i - 1;
        if (s[i] != s[ii]) {
            return false;
        }
    }
    return true;
}

void TestEmpty() {
    Assert(IsPalindrom(""), "");
}

void TestOneSymbol() {
    Assert(IsPalindrom("a"), "a");
    Assert(IsPalindrom(" "), "' '");
    Assert(IsPalindrom("\n"), "\\n");
}

void TestSpaces() {
    Assert(IsPalindrom("\n \n"), "\\n \\n");
    Assert(IsPalindrom("   "), "'   '");
    Assert(IsPalindrom(" lel \nvav\n lel "), "' lel \\nvav\\n lel '");

    Assert(!IsPalindrom("\n "), "\\n ");
    Assert(!IsPalindrom("l eve  l"), "l eve  l");
    Assert(!IsPalindrom("l eve l "), "'l eve l '");
    Assert(!IsPalindrom(" l eve l"), "' l eve l'");
    Assert(!IsPalindrom("madam\n madam"), "madam\\n madam");
    Assert(!IsPalindrom("abcd cba"), "abcd cba");
}

void TestPalindrom() {
    Assert(IsPalindrom("wasitacaroracatisaw"), "wasitacaroracatisaw");
    Assert(IsPalindrom("adda"), "adda");
    Assert(IsPalindrom("madam"), "madam");
    Assert(IsPalindrom("level"), "level");
}

void TestNonPalindrom() {
    Assert(!IsPalindrom("abcabc"), "abcabc");
    Assert(!IsPalindrom("abcdabc"), "abcdabc");
    Assert(!IsPalindrom("abcdef"), "abcdef");
    Assert(!IsPalindrom("amadam"), "amadam");
    Assert(!IsPalindrom("madama"), "madama");
    Assert(!IsPalindrom("abcdfcba"), "abcdfcba");
}

int main() {
    TestRunner runner;
    runner.RunTest(TestEmpty, "TestEmpty");
    runner.RunTest(TestOneSymbol, "TestOneSymbol");
    runner.RunTest(TestSpaces, "TestSpaces");
    runner.RunTest(TestPalindrom, "TestPalindrom");
    runner.RunTest(TestNonPalindrom, "TestNonPalindrom");
    return 0;
}
