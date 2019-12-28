#include <iostream>
#include <string>
#include <map>
#include <set>

class Synonyms {
public:
    void add(const std::string& w1, const std::string& w2) {
        synonyms[w1].insert(w2);
        synonyms[w2].insert(w1);
    }
    bool check(const std::string& w1, const std::string& w2) const {
        return synonyms.count(w1) && synonyms.at(w1).count(w2);
    }
    int count(const std::string& w) const {
        return synonyms.count(w) ? synonyms.at(w).size() : 0;
    }

private:
    std::map<std::string, std::set<std::string>> synonyms;
};

int main() {
    int n;
    std::cin >> n;
    Synonyms synonyms;
    for (int i = 0; i < n; ++i) {
        std::string op, param1, param2;
        std::cin >> op;
        if (op == "ADD") {
            std::cin >> param1 >> param2;
            synonyms.add(param1, param2);
        } else if (op == "CHECK") {
            std::cin >> param1 >> param2;
            std::cout << (synonyms.check(param1, param2) ? "YES" : "NO") << std::endl;
        } else if (op == "COUNT") {
            std::cin >> param1;
            std::cout << synonyms.count(param1) << std::endl;
        }
    }
    return 0;
}
