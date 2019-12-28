#include <vector>
#include <algorithm>
#include <iostream>

class Part {
public:
    Part(char _op, double _val)
        : op(_op)
        , val(_val)
    {}
    void Invert() {
        switch (op) {
        case '+': op = '-'; break;
        case '-': op = '+'; break;
        case '*': op = '/'; break;
        case '/': op = '*'; break;
        default: break;
        }
    }
    double Apply(double src) const {
        switch (op) {
        case '+': return src + val;
        case '-': return src - val;
        case '*': return src * val;
        case '/': return src / val;
        default: break;
        }
        return 0;
    }

private:
    char op;
    double val;
};

class Function {
public:
    void AddPart(char op, double val) {
        parts.emplace_back(op, val);
    }
    void Invert() {
        for (auto& p : parts) {
            p.Invert();
        }
        std::reverse(std::begin(parts), std::end(parts));
    }
    double Apply(double val) const {
        for (const auto& p : parts) {
            val = p.Apply(val);
        }
        return val;
    }

private:
    std::vector<Part> parts;
};

/*struct Image {
    double quality;
    double freshness;
    double rating;
};

struct Params {
    double a;
    double b;
    double c;
};

Function MakeWeightFunction(const Params& params, const Image& image) {
    //Function function;
    //function.AddPart('-', image.freshness * params.a + params.b);
    //function.AddPart('+', image.rating * params.c);
    //return function;
    Function function;
    function.AddPart('*', params.a);
    function.AddPart('-', image.freshness * params.b);
    function.AddPart('+', image.rating * params.c);
    return function;
}

double ComputeImageWeight(const Params& params, const Image& image) {
    Function function = MakeWeightFunction(params, image);
    return function.Apply(image.quality);
}

double ComputeQualityByWeight(const Params& params, const Image& image, double weight) {
    Function function = MakeWeightFunction(params, image);
    function.Invert();
    return function.Apply(weight);
}

int main() {
    Image image = { 10, 2, 6 };
    Params params = { 4, 2, 6 };
    std::cout << ComputeImageWeight(params, image) << std::endl;
    std::cout << ComputeQualityByWeight(params, image, 52) << std::endl;
    //std::cout << ComputeQualityByWeight(params, image, 46) << std::endl;
    return 0;
}*/

