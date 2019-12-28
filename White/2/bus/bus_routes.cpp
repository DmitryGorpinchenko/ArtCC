#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>

void Log(const std::string& msg) {
    std::cout << msg << std::endl;
}

template <typename Route>
class BusRoutes {
public:
    void add(const Route& route) {
        addImpl(route);
    }

private:
    void addImpl(const std::vector<std::string>& route) {
        if (routes.count(route)) {
            Log("Already exists for " + std::to_string(routes.at(route)));
        } else {
            routes.emplace(route, next);
            Log("New bus " + std::to_string(next));
            ++next;
        }
    }
    template <typename Collection>
    void addImpl(const Collection& route) {
        const std::vector<std::string> tmp = { std::begin(route), std::end(route) };
        addImpl(tmp);
    }

    std::map<std::vector<std::string>, int> routes;
    int next = 1;
};

void bus2() {
    int n;
    std::cin >> n;
    using Route = std::vector<std::string>;
    BusRoutes<Route> routes;
    for (int i = 0; i < n; ++i) {
        int m;
        std::cin >> m;
        Route route;
        route.reserve(m);
        for (int j = 0; j < m; ++j) {
            std::string stop;
            std::cin >> stop;
            route.push_back(stop);
        }
        routes.add(route);
    }
}

void bus3() {
    int n;
    std::cin >> n;
    using Route = std::set<std::string>;
    BusRoutes<Route> routes;
    for (int i = 0; i < n; ++i) {
        int m;
        std::cin >> m;
        Route route;
        for (int j = 0; j < m; ++j) {
            std::string stop;
            std::cin >> stop;
            route.insert(stop);
        }
        routes.add(route);
    }
}

int main() {
    //bus2();
    bus3();
    return 0;
}
