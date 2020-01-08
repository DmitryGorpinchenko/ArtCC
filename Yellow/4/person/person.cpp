#include <iostream>
#include <string>
#include <map>
#include <iterator>

using namespace std;

string Name(int year, const map<int, string>& names) {
    auto it = names.upper_bound(year);
    return (it == begin(names)) ? "" : (*prev(it)).second;
}

class Person {
public:
    void ChangeFirstName(int year, const string& first_name) {
        first_names[year] = first_name;
    }
    void ChangeLastName(int year, const string& last_name) {
        last_names[year] = last_name;
    }
    string GetFullName(int year) const {
        const auto fn = Name(year, first_names);
        const auto ln = Name(year, last_names);
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

private:
    map<int, string> first_names;
    map<int, string> last_names;
};

int main() {
    Person person;
    person.ChangeFirstName(1965, "Polina");
    person.ChangeLastName(1967, "Sergeeva");
    for (int year : {1900, 1965, 1990}) {
        cout << person.GetFullName(year) << endl;
    }
    person.ChangeFirstName(1970, "Appolinaria");
    for (int year : {1969, 1970}) {
        cout << person.GetFullName(year) << endl;
    }
    person.ChangeLastName(1968, "Volkova");
    for (int year : {1969, 1970}) {
        cout << person.GetFullName(year) << endl;
    }
    return 0;
}

