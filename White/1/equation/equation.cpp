#include <iostream>
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

int main() {
    double a, b, c;
    std::cin >> a >> b >> c;
    for (auto r : Roots(a, b, c)) {
        std::cout << r << " ";
    }
    std::cout << std::endl;
    return 0;
}
