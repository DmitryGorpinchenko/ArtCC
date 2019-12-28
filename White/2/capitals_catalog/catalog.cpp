#include <iostream>
#include <string>
#include <map>

void Log(const std::string& msg) {
    std::cout << msg << std::endl;
}

class CapitalsCatalog {
public:
    void changeCapital(const std::string& country, const std::string& cap) {
        if (!has(country)) {
            capitals.emplace(country, cap);
            Log("Introduce new country " + country + " with capital " + cap);
        } else if (capital(country) == cap) {
            Log("Country " + country + " hasn't changed its capital");
        } else {
            Log("Country " + country + " has changed its capital from " + capital(country) + " to " + cap);
            capitals[country] = cap;
        }
    }
    void renameCountry(const std::string& from, const std::string& to) {
        if ((from == to) || (!has(from)) || (has(to))) {
            Log("Incorrect rename, skip");
            return;
        }
        const auto cap = capital(from);
        capitals.emplace(to, cap);
        capitals.erase(from);
        Log("Country " + from + " with capital " + cap + " has been renamed to " + to);
    }

    std::string about(const std::string& country) const {
        if (!has(country)) {
            return "Country " + country + " doesn't exist";
        }
        return "Country " + country + " has capital " + capital(country);
    }
    std::string dump() const {
        if (capitals.empty()) {
            return "There are no countries in the world";
        }
        std::string res;
        for (const auto& p : capitals) {
            res += (p.first + "/" + p.second + " ");
        }
        return res;
    }

private:
    bool has(const std::string& country) const { return capitals.count(country); }
    std::string capital(const std::string& country) const { return capitals.at(country); }

    std::map<std::string, std::string> capitals;
};

int main() {
    int n;
    std::cin >> n;
    CapitalsCatalog catalog;
    for (int i = 0; i < n; ++i) {
        std::string op, param1, param2;
        std::cin >> op;
        if (op == "CHANGE_CAPITAL") {
            std::cin >> param1 >> param2;
            catalog.changeCapital(param1, param2);
        } else if (op == "RENAME") {
            std::cin >> param1 >> param2;
            catalog.renameCountry(param1, param2);
        } else if (op == "ABOUT") {
            std::cin >> param1;
            std::cout << catalog.about(param1) << std::endl;
        } else if (op == "DUMP") {
            std::cout << catalog.dump() << std::endl;
        }
    }
    return 0;
}
