#include <iostream>

double salePrice(double price, double discount) {
    return price * (1. - discount / 100.);
}

int main() {
    double n, a, b, x, y;
    std::cin >> n >> a >> b >> x >> y;

    if (n > b) {
        n = salePrice(n, y);
    } else if (n > a) {
        n = salePrice(n, x);
    }

    std::cout << n << std::endl;
    return 0;
}
