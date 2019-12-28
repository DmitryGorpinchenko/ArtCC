#include <iostream>
#include <vector>
#include <string>

struct Student {
    std::string first_name;
    std::string last_name;
    int day = 0;
    int month = 0;
    int year = 0;
};

std::istream& operator >>(std::istream& in, Student& out) {
    in >> out.first_name >> out.last_name >> out.day >> out.month >> out.year;
    return in;
}

int main() {
    int n = 0;
    std::cin >> n;
    std::vector<Student> students(n);
    for (auto& s : students) {
        std::cin >> s;
    }
    int m = 0;
    std::cin >> m;
    for (int i = 0; i < m; ++i) {
        int k = 0;
        std::string op;
        std::cin >> op >> k;
        if (k < 1 || k > n) {
            std::cout << "bad request";
        } else if (op == "name") {
            const auto& s = students[k - 1];
            std::cout << s.first_name << ' ' << s.last_name;
        } else if (op == "date") {
            const auto& s = students[k - 1];
            std::cout << s.day << '.' << s.month << '.' << s.year;
        } else {
            std::cout << "bad request";
        }
        std::cout << std::endl;
    }
    return 0;
}
