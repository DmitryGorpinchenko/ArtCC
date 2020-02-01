#include <string>

class Animal {
public:
    virtual ~Animal() {}

    virtual std::string Voice() const {
        return "Not implemented yet";
    }
};

class Tiger: public Animal {
    std::string Voice() const override {
        return "Rrrr";
    }
};

class Wolf: public Animal {
    std::string Voice() const override {
        return "Wooo";
    }
};

class Fox: public Animal {
    std::string Voice() const override {
        return "Tyaf";
    }
};

