#include "rational.h"

#include <cmath>

// a, b - non-negative
int gcd(int a, int b) {
    auto max = (b > a) ? b : a;
    auto min = (b > a) ? a : b;
    while (min > 0) {
        const auto mod = max % min;
        max = min;
        min = mod;
    }
    return max;
}

Rational::Rational(int numerator, int denominator) {
    if (numerator == 0) {
        denominator = 1;
    }
    if (denominator < 0) {
        numerator = -numerator;
        denominator = -denominator;
    }
    const auto div = gcd(abs(numerator), denominator);
    num = numerator / div;
    denom = denominator / div;
}

int Rational::Numerator() const { return num; }
int Rational::Denominator() const { return denom; }
