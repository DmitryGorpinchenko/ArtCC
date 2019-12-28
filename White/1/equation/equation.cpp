#include <iostream>
#include <vector>
#include <cmath>

std::vector<double> roots(double a, double b, double c) {
    const auto d = b*b - 4*a*c;
    if (d < 0.) {
        return {};
    }
    if (a == 0.) {
        if (b == 0.) {
            return {};
        }
        return { -c/b };
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
    for (auto r : roots(a, b, c)) {
        std::cout << r << " ";
    }
    std::cout << std::endl;
    return 0;
}
