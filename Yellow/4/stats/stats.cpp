#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*enum class Gender {
    FEMALE,
    MALE
};

struct Person {
    int age;
    Gender gender;
    bool is_employed;
};

template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
    if (range_begin == range_end) {
        return 0;
    }
    vector<typename InputIt::value_type> range_copy(range_begin, range_end);
    auto middle = begin(range_copy) + range_copy.size() / 2;
    nth_element(begin(range_copy), middle, end(range_copy), [](const Person& lhs, const Person& rhs) {
        return lhs.age < rhs.age;
    });
    return middle->age;
}*/

void PrintStats(vector<Person> persons) {
    const auto unemployed = [](const Person& p) { return !p.is_employed; };
    auto males_begin = partition(begin(persons), end(persons), [](const Person& p) { return p.gender == Gender::FEMALE; });
    auto employed_males_begin = partition(males_begin, end(persons), unemployed);
    auto females_begin = begin(persons);
    auto employed_females_begin = partition(females_begin, males_begin, unemployed);

    cout << "Median age = " << ComputeMedianAge(begin(persons), end(persons)) << endl;
    cout << "Median age for females = " << ComputeMedianAge(females_begin, males_begin) << endl;
    cout << "Median age for males = " << ComputeMedianAge(males_begin, end(persons)) << endl;
    cout << "Median age for employed females = " << ComputeMedianAge(employed_females_begin, males_begin) << endl;
    cout << "Median age for unemployed females = " << ComputeMedianAge(females_begin, employed_females_begin) << endl;
    cout << "Median age for employed males = " << ComputeMedianAge(employed_males_begin, end(persons)) << endl;
    cout << "Median age for unemployed males = " << ComputeMedianAge(males_begin, employed_males_begin) << endl;
}
