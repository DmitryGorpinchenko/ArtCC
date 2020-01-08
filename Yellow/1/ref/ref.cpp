#include <iostream>
#include <stdexcept>
#include <map>
using namespace std;

template <typename K, typename V>
V& GetRefStrict(map<K, V>& m, const K& k) {
    auto it = m.find(k);
    if (it == end(m)) {
        throw runtime_error("key does not exist");
    }
    return (*it).second;
}

int main() {
    map<int, string> m = {{0, "value"}};
    string& item = GetRefStrict(m, 0);
    item = "newvalue";
    cout << m[0] << endl;
    return 0;
}
