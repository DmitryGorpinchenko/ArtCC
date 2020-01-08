#include "test_runner.h"

#include <vector>
#include <cmath>

std::vector<double> Roots(double a, double b, double c) {
    // linear
    if (a == 0.) {
        if (b == 0.) {
            return {};
        }
        return { -c/b };
    }
    // quadratic
    const auto d = b*b - 4*a*c;
    if (d < 0.) {
        return {};
    }
    if (d == 0.) {
        return { -b/(2*a) };
    }
    const auto d_sqrt = sqrt(d);
    return { (-b + d_sqrt) / (2*a),
             (-b - d_sqrt) / (2*a) };
}

int GetDistinctRealRootCount(double a, double b, double c) {
    return Roots(a, b, c).size();
}

void TestLinear() {
    AssertEqual(GetDistinctRealRootCount(0, 1, 0), 1, "Coefs: 0, 1, 0");
    AssertEqual(GetDistinctRealRootCount(0, 1, 2), 1, "Coefs: 0, 1, 2");
    AssertEqual(GetDistinctRealRootCount(0, 0, 2), 0, "Coefs: 0, 0, 2");
    AssertEqual(GetDistinctRealRootCount(0, -4, 0), 1, "Coefs: 0, -4, 0");
    AssertEqual(GetDistinctRealRootCount(0, 2, -4), 1, "Coefs: 0, 1, -2");
}
void TestQuadratic() {
    AssertEqual(GetDistinctRealRootCount(1, 0, 0), 1, "Coefs: 1, 0, 0");
    AssertEqual(GetDistinctRealRootCount(1, 0, 1), 0, "Coefs: 1, 0, 1");
    AssertEqual(GetDistinctRealRootCount(1, 0, -4), 2, "Coefs: 1, 0, -4");
    AssertEqual(GetDistinctRealRootCount(-4, 0, 0), 1, "Coefs: -4, 0, 0");
    AssertEqual(GetDistinctRealRootCount(-1, 0, -4), 0, "Coefs: -1, 0, -4");
    AssertEqual(GetDistinctRealRootCount(-1, 0, 4), 2, "Coefs: -1, 0, 4");
    AssertEqual(GetDistinctRealRootCount(1, 4, 4), 1, "Coefs: 1, 4, 4");
    AssertEqual(GetDistinctRealRootCount(1, -4, 4), 1, "Coefs: 1, -4, 4");
}

int main() {
    TestRunner runner;
    runner.RunTest(TestLinear, "TestLinear");
    runner.RunTest(TestQuadratic, "TestQuadratic");
    return 0;
}
