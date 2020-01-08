#pragma once

class Rational {
public:
    Rational(int numerator = 0, int denominator = 1);

    int Numerator() const;
    int Denominator() const;

private:
    int num;
    int denom;
};
