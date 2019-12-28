#include <iostream>
#include <vector>
#include <map>
#include <string>

class SortedStrings {
public:
    void AddString(const std::string& s) { ++strings[s]; }
    std::vector<std::string> GetSortedStrings() const {
        std::vector<std::string> res;
        for (const auto& p : strings) {
            res.insert(std::end(res), p.second, p.first);
        }
        return res;
    }
private:
    std::map<std::string, int> strings;
};

void PrintSortedStrings(const SortedStrings& strings) {
    for (const auto& s : strings.GetSortedStrings()) {
        std::cout << s << " ";
    }
    std::cout << std::endl;
}

int main() {
    SortedStrings strings;

    strings.AddString("first");
    strings.AddString("third");
    strings.AddString("second");
    PrintSortedStrings(strings);

    strings.AddString("second");
    PrintSortedStrings(strings);

    return 0;
}

