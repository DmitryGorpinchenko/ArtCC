#include <iostream>
#include <string>

const std::string& min(const std::string& s1, const std::string& s2) {
    return (s1 < s2) ? s1 : s2;
}

const std::string& min(const std::string& s1, const std::string& s2, const std::string& s3) {
    return min(min(s1, s2), s3);
}

int main() {
    std::string s1, s2, s3;
    std::cin >> s1 >> s2 >> s3;
    std::cout << min(s1, s2, s3) << std::endl;
    return 0;
}
