#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <cmath>

using namespace std;

const auto PI = 3.14;
const auto RECT = "RECT";
const auto TRIANGLE = "TRIANGLE";
const auto CIRCLE = "CIRCLE";

class Figure {
public:
    virtual string Name() const = 0;
    virtual double Perimeter() const = 0;
    virtual double Area() const = 0;
};

class Triangle : public Figure {
public:
    Triangle(double _a, double _b, double _c)
        : a(_a)
        , b(_b)
        , c(_c)
    {}

    string Name() const override {
        return TRIANGLE;
    }
    double Perimeter() const override {
        return a + b + c;
    }
    double Area() const override {
        const auto half_p = Perimeter() / 2;
        return sqrt(half_p * (half_p - a) * (half_p - b) * (half_p - c));
    }
private:
    double a, b, c;
};

class Rect : public Figure {
public:
    Rect(double _w, double _h)
        : w(_w)
        , h(_h)
    {}

    string Name() const override {
        return RECT;
    }
    double Perimeter() const override {
        return 2 * (w + h);
    }
    double Area() const override {
        return w * h;
    }
private:
    double w, h;
};

class Circle : public Figure {
public:
    Circle(double _r)
        : r(_r)
    {}

    string Name() const override {
        return CIRCLE;
    }
    double Perimeter() const override {
        return 2 * PI * r;
    }
    double Area() const override {
        return PI * r * r;
    }
private:
    double r;
};

shared_ptr<Figure> CreateFigure(istream& is) {
    string type;
    is >> type;
    if (type == RECT) {
        double w, h;
        is >> w >> h;
        return make_shared<Rect>(w, h);
    } else if (type == TRIANGLE) {
        double a, b, c;
        is >> a >> b >> c;
        return make_shared<Triangle>(a, b, c);
    } else if (type == CIRCLE) {
        double r;
        is >> r;
        return make_shared<Circle>(r);
    }
    return {};
}

int main() {
    vector<shared_ptr<Figure>> figures;
    for (string line; getline(cin, line); ) {
        istringstream is(line);
        string command;
        is >> command;
        if (command == "ADD") {
            figures.push_back(CreateFigure(is));
        } else if (command == "PRINT") {
            for (const auto& current_figure : figures) {
                cout << fixed << setprecision(3)
                << current_figure->Name() << " "
                << current_figure->Perimeter() << " "
                << current_figure->Area() << endl;
            }
        }
    }
    return 0;
}
