#include <iostream>
#include <string>
#include <algorithm>

class ReversibleString {
public:
    ReversibleString(const std::string& _s = "") : s(_s) {}

    void Reverse() {
        rev = !rev;
    }

    std::string ToString() const {
        if (rev) {
            std::reverse(std::begin(s), std::end(s));
            rev = false;
        }
        return s;
    }

private:
    mutable std::string s;
    mutable bool rev = false;
};

int main() {
    ReversibleString s("live");
    s.Reverse();
    std::cout << s.ToString() << std::endl;

    s.Reverse();
    const ReversibleString& s_ref = s;
    std::string tmp = s_ref.ToString();
    std::cout << tmp << std::endl;

    ReversibleString empty;
    std::cout << '"' << empty.ToString() << '"' << std::endl;

    return 0;
}
