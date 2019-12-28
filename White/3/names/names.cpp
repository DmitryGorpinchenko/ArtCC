#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>

std::vector<std::string> History(int upper_bound, const std::map<int, std::string>& m) {
    std::vector<std::string> h;
    for (const auto& p : m) {
        if (upper_bound < p.first) {
            break;
        }
        if (h.empty() || (h.back() != p.second)) {
            h.push_back(p.second);
        }
    }
    return h;
}

std::string ToStr(const std::vector<std::string>& history) {
    const auto sz = history.size();
    if (sz == 1) {
        return history.front();
    }
    std::string res = (history.back() + " (");
    for (int i = sz - 2; i > 0; --i) {
        res += (history[i] + ", ");
    }
    res += (history.front() + ")");
    return res;
}

class Person {
public:
    Person() = default; // should be commented out when submit 3rd assignment
    Person(const std::string& first_name, const std::string& last_name, int year)
        : birth_year(year)
    {
        ChangeFirstName(year, first_name);
        ChangeLastName(year, last_name);
    }

    void ChangeFirstName(int year, const std::string& first_name) {
        first_names[year] = first_name;
    }
    void ChangeLastName(int year, const std::string& last_name) {
        last_names[year] = last_name;
    }
    std::string GetFullName(int year) const {
        if (year < birth_year) {
            return "No person";
        }
        auto fn_it = first_names.upper_bound(year);
        auto ln_it = last_names.upper_bound(year);
        const auto fn = (fn_it == std::begin(first_names)) ? "" : (*std::prev(fn_it)).second;
        const auto ln = (ln_it == std::begin(last_names)) ? "" : (*std::prev(ln_it)).second;
        if (fn.empty() && ln.empty()) {
            return "Incognito";
        }
        if (fn.empty()) {
            return ln + " with unknown first name";
        }
        if (ln.empty()) {
            return fn + " with unknown last name";
        }
        return fn + " " + ln;
    }
    std::string GetFullNameWithHistory(int year) const {
        if (year < birth_year) {
            return "No person";
        }
        auto f_names = History(year, first_names);
        auto l_names = History(year,last_names);
        if (f_names.empty() && l_names.empty()) {
            return "Incognito";
        }
        if (f_names.empty()) {
            return ToStr(l_names) + " with unknown first name";
        }
        if (l_names.empty()) {
            return ToStr(f_names) + " with unknown last name";
        }
        return ToStr(f_names) + " " + ToStr(l_names);
    }

private:
    std::map<int, std::string> first_names;
    std::map<int, std::string> last_names;
    int birth_year = 0;
};

void test1() {
    Person person;
    person.ChangeFirstName(1965, "Polina");
    person.ChangeLastName(1967, "Sergeeva");
    for (int year : { 1900, 1965, 1990 }) {
        std::cout << person.GetFullName(year) << std::endl;
    }
    person.ChangeFirstName(1970, "Appolinaria");
    for (int year : { 1969, 1970 }) {
        std::cout << person.GetFullName(year) << std::endl;
    }
    person.ChangeLastName(1968, "Volkova");
    for (int year : { 1969, 1970 }) {
        std::cout << person.GetFullName(year) << std::endl;
    }
}

void test2() {
    Person person;
    person.ChangeFirstName(1900, "Eugene");
    person.ChangeLastName(1900, "Sokolov");
    person.ChangeLastName(1910, "Sokolov");
    person.ChangeFirstName(1920, "Evgeny");
    person.ChangeLastName(1930, "Sokolov");
    std::cout << person.GetFullNameWithHistory(1940) << std::endl;
}

void test3() {
    Person person;
    person.ChangeFirstName(1965, "Polina");
    person.ChangeLastName(1967, "Sergeeva");
    for (int year : { 1900, 1965, 1990 }) {
        std::cout << person.GetFullNameWithHistory(year) << std::endl;
    }
    person.ChangeFirstName(1970, "Appolinaria");
    for (int year : { 1969, 1970 }) {
        std::cout << person.GetFullNameWithHistory(year) << std::endl;
    }
    person.ChangeLastName(1968, "Volkova");
    for (int year : { 1969, 1970 }) {
        std::cout << person.GetFullNameWithHistory(year) << std::endl;
    }
    person.ChangeFirstName(1990, "Polina");
    person.ChangeLastName(1990, "Volkova-Sergeeva");
    std::cout << person.GetFullNameWithHistory(1990) << std::endl;
    person.ChangeFirstName(1966, "Pauline");
    std::cout << person.GetFullNameWithHistory(1966) << std::endl;
    person.ChangeLastName(1960, "Sergeeva");
    for (int year : { 1960, 1967 }) {
        std::cout << person.GetFullNameWithHistory(year) << std::endl;
    }
    person.ChangeLastName(1961, "Ivanova");
    std::cout << person.GetFullNameWithHistory(1967) << std::endl;
}

void test4() {
    Person person("Polina", "Sergeeva", 1960);
    for (int year : { 1959, 1960 }) {
        std::cout << person.GetFullNameWithHistory(year) << std::endl;
    }
    person.ChangeFirstName(1965, "Appolinaria");
    person.ChangeLastName(1967, "Ivanova");
    for (int year : { 1965, 1967 }) {
        std::cout << person.GetFullNameWithHistory(year) << std::endl;
    }
}

int main() {
    //test1();
    //test2();
    test3();
    //test4();
    return 0;
}

