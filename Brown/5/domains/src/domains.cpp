#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <memory>
using namespace std;

class DNS {
    struct Node {
        bool valid = false;
        unordered_map<string, unique_ptr<Node>> children;
    };
public:
    using Domain = vector<string>;

    DNS(vector<Domain> domains) {
        for (auto& d : domains) {
            Add(move(d));
        }
    }

    bool IsSubdomain(const Domain& d) const {
        auto cur = root.get();
        for (auto it = rbegin(d); it != rend(d); ++it) {
            const auto& children = cur->children;
            auto child_it = children.find(*it);
            if (child_it == end(children)) {
                break;
            }
            auto child = (*child_it).second.get();
            if (child->valid) {
                return true;
            }
            cur = child;
        }
        return false;
    }

private:
    void Add(Domain d) {
        auto cur = root.get();
        auto it = rbegin(d);
        while (it != rend(d)) {
            const auto& children = cur->children;
            auto child_it = children.find(*it);
            if (child_it == end(children)) {
                break;
            }
            cur = (*child_it).second.get();
            ++it;
        }
        while (it != rend(d)) {
            auto& children = cur->children;
            const auto& [iter, flag] = children.emplace(move(*it), make_unique<Node>());
            cur = (*iter).second.get();
            ++it;
        }
        cur->valid = true;
    }

    unique_ptr<Node> root = make_unique<Node>();
};

vector<string_view> Split(string_view str, char delim = ' ') {
    vector<string_view> res;
    do {
        size_t lpos = str.find_first_not_of(delim);
        if (lpos == str.npos) {
            break;
        }
        size_t rpos = str.find(delim, lpos + 1);
        res.push_back(str.substr(lpos, rpos - lpos));
        if (rpos == str.npos) {
            break;
        }
        str.remove_prefix(rpos + 1);
    } while (true);
    return res;
}

DNS::Domain Domain(string_view str) {
    const auto parts = Split(str, '.');
    return { begin(parts), end(parts) };
}

vector<DNS::Domain> ReadDomains(istream& is) {
    size_t count;
    is >> count;
    vector<DNS::Domain> domains;
    domains.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        string domain;
        is >> domain;
        domains.push_back(Domain(domain));
    }
    return domains;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    DNS black_list(ReadDomains(cin));
    for (const auto& d : ReadDomains(cin)) {
        cout << (black_list.IsSubdomain(d) ? "Bad" : "Good") << '\n';
    }
    return 0;
}

