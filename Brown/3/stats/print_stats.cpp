#include <algorithm>
#include <iostream>
#include <vector>
#include <optional>
#include <string>
#include <numeric>
#include <utility>

using namespace std;

struct Person {
    string name;
    int age, income;
    bool is_male;
};

vector<Person> ReadPeople(istream& input) {
    int count;
    input >> count;

    vector<Person> result(count);
    for (Person& p : result) {
        char gender;
        input >> p.name >> p.age >> p.income >> gender;
        p.is_male = (gender == 'M');
    }

    return result;
}

template <typename It>
optional<string> MostFreqName(It first, It last) {
    optional<string> res;
    if (first != last) {
        sort(first, last, [](const auto& lhs, const auto& rhs) {
            return lhs.name < rhs.name;
        });
        const auto pred = [](const auto& lhs, const auto& rhs) {
            return lhs.name != rhs.name;
        };
        auto l = adjacent_find(first, last, pred);
        res = (*first).name;
        int max_count = distance(first, l) + (l != last);
        while (l != last) {
            auto r = adjacent_find(next(l), last, pred);
            int count = distance(next(l), r) + (r != last);
            if (max_count < count) {
                res = (*next(l)).name;
                max_count = count;
            }
            l = r;
        }
    }
    return res;
}

void Run(const vector<Person>& sorted_by_age,
         const vector<int>& income_sums,
         const pair<optional<string>, optional<string>>& m_w_name)
{
    for (string command; cin >> command; ) {
        if (command == "AGE") {
            int adult_age;
            cin >> adult_age;

            auto adult_begin = lower_bound(
                begin(sorted_by_age), end(sorted_by_age), adult_age, [](const Person& lhs, int age) {
                    return lhs.age < age;
                }
            );

            cout << "There are " << std::distance(adult_begin, end(sorted_by_age))
                 << " adult people for maturity age " << adult_age << '\n';
        } else if (command == "WEALTHY") {
            int count;
            cin >> count;
            
            cout << "Top-" << count << " people have total income " << income_sums[count - 1] << '\n';
        } else if (command == "POPULAR_NAME") {
            char gender;
            cin >> gender;

            const auto& name = (gender == 'M') ? m_w_name.first : m_w_name.second;
            if (!name) {
                cout << "No people of gender " << gender;
            } else {
                cout << "Most popular name among people of gender " << gender << " is " << *name;
            }
            cout << '\n';
        }
    }
}

int main() {
    vector<Person> people = ReadPeople(cin);

    const vector<Person> sorted_by_age = [&people]() {
        sort(begin(people), end(people), [](const Person& lhs, const Person& rhs) {
            return lhs.age < rhs.age;
        });
        return people;
    }();
    const vector<int> income_sums = [&people]() {
        sort(begin(people), end(people), [](const Person& lhs, const Person& rhs) {
            return lhs.income > rhs.income;
        });
        vector<int> prefix_sums(people.size(), 0);
        if (!prefix_sums.empty()) {
            prefix_sums[0] = people[0].income;
            for (size_t i = 1; i < prefix_sums.size(); ++i) {
                prefix_sums[i] = prefix_sums[i - 1] + people[i].income;
            }
        }
        return prefix_sums;
    }();
    const pair<optional<string>, optional<string>> m_w_name = [&people]() {
        auto w_begin = partition(begin(people), end(people), [](const Person& p) {
            return p.is_male;
        });
        return make_pair(MostFreqName(begin(people), w_begin), MostFreqName(w_begin, end(people)));
    }();

    Run(sorted_by_age, income_sums, m_w_name);
    return 0;
}
