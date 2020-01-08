#include <iostream>
#include <vector>
#include <map>
#include <utility>
using namespace std;

template <typename T> T Sqr(const T& val);
template <typename T> vector<T> Sqr(const vector<T>& val);
template <typename K, typename V> map<K, V> Sqr(const map<K, V>& val);
template <typename F, typename S> pair<F, S> Sqr(const pair<F, S>& val);

template <typename T> T Sqr(const T& val) {
    return val * val;
}
template <typename T>
vector<T> Sqr(const vector<T>& val) {
    vector<T> res;
    res.reserve(val.size());
    for (const auto& el : val) {
        res.push_back(Sqr(el));
    }
    return res;
}
template <typename K, typename V>
map<K, V> Sqr(const map<K, V>& val) {
    map<K, V> res;
    for (const auto& p : val) {
        res.emplace(p.first, Sqr(p.second));
    }
    return res;
}
template <typename F, typename S>
pair<F, S> Sqr(const pair<F, S>& val) {
    return { Sqr(val.first), Sqr(val.second) };
}

int main() {
    vector<int> v = { 1, 2, 3 };
    cout << "vector:";
    for (int x : Sqr(v)) {
        cout << ' ' << x;
    }
    cout << endl;

    map<int, pair<int, int>> map_of_pairs = {
        { 4, { 2, 2 } },
        { 7, { 4, 3 } }
    };
    cout << "map of pairs:" << endl;
    for (const auto& x : Sqr(map_of_pairs)) {
        cout << x.first << ' ' << x.second.first << ' ' << x.second.second << endl;
    }
    return 0;
}

