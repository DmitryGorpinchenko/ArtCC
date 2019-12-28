#include <iostream>
#include <map>
#include <vector>
#include <string>

class BusRoutesCatalog {
public:
    void add(const std::string& bus, const std::vector<std::string>& stops) {
        if (stops_for_bus.emplace(bus, stops).second) {
            for (const auto& s : stops) {
                buses_for_stop[s].push_back(bus);
            }
        }
    }
    const std::vector<std::string>& busesForStop(const std::string& stop) const {
        if (buses_for_stop.count(stop)) {
            return buses_for_stop.at(stop);
        }
        static const std::vector<std::string> null;
        return null;
    }
    const std::vector<std::string>& stopsForBus(const std::string& bus) const {
        if (stops_for_bus.count(bus)) {
            return stops_for_bus.at(bus);
        }
        static const std::vector<std::string> null;
        return null;
    }
    const std::map<std::string, std::vector<std::string>>& allBuses() const {
        return stops_for_bus;
    }

private:
    std::map<std::string, std::vector<std::string>> stops_for_bus;
    std::map<std::string, std::vector<std::string>> buses_for_stop;
};

void BusesForStop(const std::string& stop, const BusRoutesCatalog& catalog) {
    const auto& buses = catalog.busesForStop(stop);
    if (buses.empty()) {
        std::cout << "No stop";
    } else {
        for (const auto& b : buses) {
            std::cout << b << " ";
        }
    }
    std::cout << std::endl;
}

void StopsForBus(const std::string& bus, const BusRoutesCatalog& catalog) {
    const auto& stops = catalog.stopsForBus(bus);
    if (stops.empty()) {
        std::cout << "No bus" << std::endl;
    } else {
        for (const auto& s : stops) {
            std::cout << "Stop " << s << ": ";
            const auto& buses = catalog.busesForStop(s);
            if (buses.size() == 1) {
                std::cout << "no interchange" << std::endl;
            } else {
                for (const auto& b : buses) {
                    if (b != bus) {
                        std::cout << b << " ";
                    }
                }
                std::cout << std::endl;
            }
        }
    }
}

void AllBuses(const BusRoutesCatalog& catalog) {
    const auto& all_buses = catalog.allBuses();
    if (all_buses.empty()) {
        std::cout << "No buses" << std::endl;
    } else {
        for (const auto& p : all_buses) {
            std::cout << "Bus " << p.first << ": ";
            for (const auto& s : p.second) {
                std::cout << s << " ";
            }
            std::cout << std::endl;
        }
    }
}

int main() {
    int n;
    std::cin >> n;
    BusRoutesCatalog catalog;
    for (int i = 0; i < n; ++i) {
        std::string op;
        std::cin >> op;
        if (op == "NEW_BUS") {
            int stops_count;
            std::string bus;
            std::cin >> bus;
            std::cin >> stops_count;
            std::vector<std::string> stops;
            stops.reserve(stops_count);
            for (int j = 0; j < stops_count; ++j) {
                std::string stop;
                std::cin >> stop;
                stops.push_back(stop);
            }
            catalog.add(bus, stops);
        } else if (op == "BUSES_FOR_STOP") {
            std::string stop;
            std::cin >> stop;
            BusesForStop(stop, catalog);
        } else if (op == "STOPS_FOR_BUS") {
            std::string bus;
            std::cin >> bus;
            StopsForBus(bus, catalog);
        } else if (op == "ALL_BUSES") {
            AllBuses(catalog);
        }
    }
    return 0;
}
