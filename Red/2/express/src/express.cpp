#include "profile.h"

#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std;

set<int>::const_iterator FindNearestElement(const set<int>& numbers, int border) {
    const auto it = numbers.lower_bound(border);
    if (it == begin(numbers)) {
        return it;
    }
    if (it == end(numbers)) {
        return prev(it);
    }
    const auto d1 = *it - border;
    const auto d2 = border - *prev(it);
    return d1 < d2 ? it : prev(it);
}

class RouteManager {
public:
    void AddRoute(int start, int finish) {
        reachable_lists_[start].insert(finish);
        reachable_lists_[finish].insert(start);
    }
    int FindNearestFinish(int start, int finish) const {
        int result = abs(start - finish);
        auto it = reachable_lists_.find(start);
        if (it != end(reachable_lists_)) {
            const auto& reachable_stations = (*it).second;
            if (!reachable_stations.empty()) {
                result = min(result, abs(finish - *FindNearestElement(reachable_stations, finish)));
            }
        }
        return result;
    }
private:
    map<int, set<int>> reachable_lists_;
};

int main() {
    RouteManager routes;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int start, finish;
        cin >> start >> finish;
        if (query_type == "ADD") {
            routes.AddRoute(start, finish);
        } else if (query_type == "GO") {
            cout << routes.FindNearestFinish(start, finish) << "\n";
        }
    }

    return 0;
}
