#include <iostream>
#include <string>
#include <vector>

const std::vector<int> days = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

class MonthlyAffairs {
public:
    MonthlyAffairs()
        : affairs(32)
        , month(0)
    {}

    void add(int day, const std::string& affair) {
        affairs[day].push_back(affair);
    }
    void next() {
        const auto prev_last_day = days.at(month);
        month = (month + 1) % 12;
        const auto last_day = days.at(month);
        for (auto i = last_day + 1; i <= prev_last_day; ++i) {
            move(i, last_day);
        }
    }

    std::vector<std::string> dump(int day) const {
        return affairs.at(day);
    }

private:
    void move(int from, int to) {
        auto& affairs_from = affairs[from];
        auto& affairs_to = affairs[to];
        affairs_to.insert(std::end(affairs_to), std::begin(affairs_from), std::end(affairs_from));
        affairs_from.clear();
    }

    std::vector<std::vector<std::string>> affairs;
    int month;
};

int main() {
    int n;
    std::cin >> n;
    MonthlyAffairs affairs;
    for (int i = 0; i < n; ++i) {
        std::string op, affair;
        int day;
        std::cin >> op;
        if (op == "ADD") {
            std::cin >> day >> affair;
            affairs.add(day, affair);
        } else if (op == "NEXT") {
            affairs.next();
        } else if (op == "DUMP") {
            std::cin >> day;
            const auto affairs_for_day = affairs.dump(day);
            std::cout << affairs_for_day.size() << " ";
            for (const auto& a : affairs_for_day) {
                std::cout << a << " ";
            }
            std::cout << std::endl;
        }
    }
    return 0;
}
