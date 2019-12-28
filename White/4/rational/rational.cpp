#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <exception>
#include <cmath>
using namespace std;

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

// a, b - non-negative
int lcm(int a, int b) {
    return (a / gcd(a, b)) * b;
}

class Rational {
public:
    Rational(int numerator = 0, int denominator = 1) {
        if (denominator == 0) {
            throw invalid_argument("denominator is equal to zero");
        }
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

    int Numerator() const { return num; }
    int Denominator() const { return denom; }

private:
    int num;
    int denom;
};

bool operator ==(const Rational& lhs, const Rational& rhs) {
    return (lhs.Numerator() == rhs.Numerator()) && (lhs.Denominator() == rhs.Denominator());
}
Rational operator +(const Rational& lhs, const Rational& rhs) {
    const auto l_denom = lhs.Denominator();
    const auto r_denom = rhs.Denominator();
    const auto denom = lcm(l_denom, r_denom);
    const auto num = (denom / l_denom) * lhs.Numerator() + (denom / r_denom) * rhs.Numerator();
    return { num, denom };
}
Rational operator -(const Rational& lhs, const Rational& rhs) {
    return lhs + Rational{ -rhs.Numerator(), rhs.Denominator() };
}
Rational operator *(const Rational& lhs, const Rational& rhs) {
    return { lhs.Numerator() * rhs.Numerator(), lhs.Denominator() * rhs.Denominator() };
}
Rational operator /(const Rational& lhs, const Rational& rhs) {
    if (rhs == 0) {
        throw domain_error("division by zero");
    }
    return lhs * Rational{ rhs.Denominator(), rhs.Numerator() };
}
bool operator <(const Rational& lhs, const Rational& rhs) {
    const auto l_denom = lhs.Denominator();
    const auto r_denom = rhs.Denominator();
    const auto denom = lcm(l_denom, r_denom);
    return (denom / l_denom * lhs.Numerator()) < (denom / r_denom * rhs.Numerator());
}
ostream& operator <<(ostream& out, const Rational& val) {
    return out << val.Numerator() << '/' << val.Denominator();
}
istream& operator >>(istream& in, Rational& val) {
    auto num = val.Numerator();
    auto denom = val.Denominator();
    in >> num;
    in.ignore(1);
    in >> denom;
    val = { num, denom };
    return in;
}

void test1() {
    {
        const Rational r(3, 10);
        if (r.Numerator() != 3 || r.Denominator() != 10) {
            cout << "Rational(3, 10) != 3/10" << endl;
            return;
        }
    }
    {
        const Rational r(8, 12);
        if (r.Numerator() != 2 || r.Denominator() != 3) {
            cout << "Rational(8, 12) != 2/3" << endl;
            return;
        }
    }
    {
        const Rational r(-4, 6);
        if (r.Numerator() != -2 || r.Denominator() != 3) {
            cout << "Rational(-4, 6) != -2/3" << endl;
            return;
        }
    }
    {
        const Rational r(4, -6);
        if (r.Numerator() != -2 || r.Denominator() != 3) {
            cout << "Rational(4, -6) != -2/3" << endl;
            return;
        }
    }
    {
        const Rational r(0, 15);
        if (r.Numerator() != 0 || r.Denominator() != 1) {
            cout << "Rational(0, 15) != 0/1" << endl;
            return;
        }
    }
    {
        const Rational defaultConstructed;
        if (defaultConstructed.Numerator() != 0 || defaultConstructed.Denominator() != 1) {
            cout << "Rational() != 0/1" << endl;
            return;
        }
    }
    cout << "1: OK" << endl;
}

void test2() {
    {
        const Rational r1(4, 6);
        const Rational r2(2, 3);
        const bool equal = r1 == r2;
        if (!equal) {
            cout << "4/6 != 2/3" << endl;
            return;
        }
    }
    {
        const Rational a(2, 3);
        const Rational b(4, 3);
        const Rational c = a + b;
        const bool equal = c == Rational(2, 1);
        if (!equal) {
            cout << "2/3 + 4/3 != 2" << endl;
            return;
        }
    }
    {
        const Rational a(5, 7);
        const Rational b(2, 9);
        const Rational c = a - b;
        const bool equal = c == Rational(31, 63);
        if (!equal) {
            cout << "5/7 - 2/9 != 31/63" << endl;
            return;
        }
    }
    {
        const Rational r = Rational(1, 2) + Rational(1, 3) - Rational(1, 4);
        const bool equal = r == Rational(7, 12);
        if (!equal) {
            cout << "1/2 + 1/3 - 1/4 != 7/12";
            return;
        }
    }
    cout << "2: OK" << endl;
}

void test3() {
    {
        const Rational a(2, 3);
        const Rational b(4, 3);
        const Rational c = a * b;
        const bool equal = c == Rational(8, 9);
        if (!equal) {
            cout << "2/3 * 4/3 != 8/9" << endl;
            return;
        }
    }
    {
        const Rational a(5, 4);
        const Rational b(15, 8);
        const Rational c = a / b;
        const bool equal = c == Rational(2, 3);
        if (!equal) {
            cout << "5/4 / 15/8 != 2/3" << endl;
            return;
        }
    }
    {
        const Rational r = Rational(1, 2) * Rational(1, 3) / Rational(1, 4);
        const bool equal = r == Rational(2, 3);
        if (!equal) {
            cout << "1/2 * 1/3 / 1/4 != 2/3";
        }
    }
    cout << "3: OK" << endl;
}

void test4() {
    {
        ostringstream output;
        output << Rational(-6, 8);
        if (output.str() != "-3/4") {
            cout << "Rational(-6, 8) should be written as \"-3/4\"" << endl;
            return;
        }
    }
    {
        istringstream input("5/7");
        Rational r;
        input >> r;
        const bool equal = r == Rational(5, 7);
        if (!equal) {
            cout << "5/7 is incorrectly read as " << r << endl;
            return;
        }
    }
    {
        istringstream input("5/7 10/8");
        Rational r1, r2;
        input >> r1 >> r2;
        bool correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
        if (!correct) {
            cout << "Multiple values are read incorrectly: " << r1 << " " << r2 << endl;
            return;
        }
        input >> r1;
        input >> r2;
        correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
        if (!correct) {
            cout << "Read from empty stream shouldn't change arguments: " << r1 << " " << r2 << endl;
            return;
        }
    }
    cout << "4: OK" << endl;
}

void test5() {
    {
        const set<Rational> rs = {{1, 2}, {1, 25}, {3, 4}, {3, 4}, {1, 2}, {2, 4}, {4, 100}};
        if (rs.size() != 3) {
            cout << "Wrong amount of items in the set" << endl;
            return;
        }
        vector<Rational> v;
        for (auto x : rs) {
            v.push_back(x);
        }
        if (v != vector<Rational>{{1, 25}, {1, 2}, {3, 4}}) {
            cout << "Rationals comparison works incorrectly" << endl;
            return;
        }
    }
    {
        map<Rational, int> count;
        ++count[{1, 2}];
        ++count[{1, 2}];
        ++count[{2, 3}];
        if (count.size() != 2) {
            cout << "Wrong amount of items in the map" << endl;
            return;
        }
    }
    cout << "5: OK" << endl;
}

void test6() {
    try {
        const Rational r(1, 0);
        cout << "Doesn't throw in case of zero denominator" << endl;
        return;
    } catch (invalid_argument&) {
    }
    try {
        const auto x = Rational(1, 2) / Rational(0, 1);
        cout << "Doesn't throw in case of division by zero" << endl;
        return;
    } catch (domain_error&) {
    }
    cout << "6: OK" << endl;
}

void calculator() {
    char op;
    Rational lhs, rhs;
    try {
        cin >> lhs >> op >> rhs;
        switch (op) {
        case '+': cout << (lhs + rhs); break;
        case '-': cout << (lhs - rhs); break;
        case '*': cout << (lhs * rhs); break;
        case '/': cout << (lhs / rhs); break;
        default: break;
        }
    } catch (invalid_argument&) {
        cout << "Invalid argument";
    } catch (domain_error&) {
        cout << "Division by zero";
    }
    cout << endl;
}

int main() {
    //test1();
    //test2();
    //test3();
    //test4();
    //test5();
    //test6();
    calculator();
    return 0;
}
