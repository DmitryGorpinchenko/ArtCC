#include "animals.h"
#include "test_runner.h"

#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

using namespace std;

using UniqueAnimal = unique_ptr<Animal>;
using Zoo = vector<UniqueAnimal>;

UniqueAnimal Create(const string& name) {
    if (name == "Tiger") {
        return make_unique<Tiger>();
    } else if (name == "Wolf") {
        return make_unique<Wolf>();
    } else if (name == "Fox") {
        return make_unique<Fox>();
    }
    throw runtime_error("Unknown animal!");
}

Zoo CreateZoo(istream& in) {
    Zoo zoo;
    string word;
    while (in >> word) {
        zoo.push_back(Create(word));
    }
    return zoo;
}

void Process(const Zoo& zoo, ostream& out) {
    for (const auto& animal : zoo) {
        out << animal->Voice() << "\n";
    }
}

void TestZoo() {
    istringstream input("Tiger Wolf Fox Tiger");
    ostringstream output;
    Process(CreateZoo(input), output);

    const string expected =
        "Rrrr\n"
        "Wooo\n"
        "Tyaf\n"
        "Rrrr\n";

    ASSERT_EQUAL(output.str(), expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestZoo);
    return 0;
}
