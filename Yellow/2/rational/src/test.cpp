#include "test_runner.h"
#include "rational.h" // comment out when submit

void TestZero() {
    {
        const Rational r;
        AssertEqual(r.Numerator(), 0);
        AssertEqual(r.Denominator(), 1);
    }
    {
        const Rational r(0, 1);
        AssertEqual(r.Numerator(), 0);
        AssertEqual(r.Denominator(), 1);
    }
    {
        const Rational r(0, -87);
        AssertEqual(r.Numerator(), 0);
        AssertEqual(r.Denominator(), 1);
    }
    {
        const Rational r(0, 87);
        AssertEqual(r.Numerator(), 0);
        AssertEqual(r.Denominator(), 1);
    }
}

void TestNegative() {
    {
        const Rational r(-2, 9);
        AssertEqual(r.Numerator(), -2);
        AssertEqual(r.Denominator(), 9);
    }
    {
        const Rational r(2, -9);
        AssertEqual(r.Numerator(), -2);
        AssertEqual(r.Denominator(), 9);
    }
    {
        const Rational r(-3, 9);
        AssertEqual(r.Numerator(), -1);
        AssertEqual(r.Denominator(), 3);
    }
    {
        const Rational r(3, -9);
        AssertEqual(r.Numerator(), -1);
        AssertEqual(r.Denominator(), 3);
    }
    {
        const Rational r(27, -9);
        AssertEqual(r.Numerator(), -3);
        AssertEqual(r.Denominator(), 1);
    }
}

void TestPositive() {
    {
        const Rational r(3, 8);
        AssertEqual(r.Numerator(), 3);
        AssertEqual(r.Denominator(), 8);
    }
    {
        const Rational r(-3, -8);
        AssertEqual(r.Numerator(), 3);
        AssertEqual(r.Denominator(), 8);
    }
    {
        const Rational r(3, 9);
        AssertEqual(r.Numerator(), 1);
        AssertEqual(r.Denominator(), 3);
    }
    {
        const Rational r(-3, -9);
        AssertEqual(r.Numerator(), 1);
        AssertEqual(r.Denominator(), 3);
    }
    {
        const Rational r(81, 9);
        AssertEqual(r.Numerator(), 9);
        AssertEqual(r.Denominator(), 1);
    }
    {
        const Rational r(-81, -9);
        AssertEqual(r.Numerator(), 9);
        AssertEqual(r.Denominator(), 1);
    }
}

int main() {
    TestRunner runner;
    runner.RunTest(TestZero, "TestZero");
    runner.RunTest(TestNegative, "TestNegative");
    runner.RunTest(TestPositive, "TestPositive");
    return 0;
}
