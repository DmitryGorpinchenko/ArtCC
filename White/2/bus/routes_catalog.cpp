#include <iostream>
#include <sstream>
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
        auto it = buses_for_stop.find(stop);
        if (it != std::end(buses_for_stop)) {
            return (*it).second;
        }
        static const std::vector<std::string> null;
        return null;
    }
    const std::vector<std::string>& stopsForBus(const std::string& bus) const {
        auto it = stops_for_bus.find(bus);
        if (it != std::end(stops_for_bus)) {
            return (*it).second;
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

std::string BusesForStop(const std::string& stop, const BusRoutesCatalog& catalog) {
    const auto& buses = catalog.busesForStop(stop);
    if (buses.empty()) {
        return "No stop";
    }
    std::ostringstream ss;
    for (const auto& b : buses) {
        ss << b << ' ';
    }
    return ss.str();
}

std::string StopsForBus(const std::string& bus, const BusRoutesCatalog& catalog) {
    const auto& stops = catalog.stopsForBus(bus);
    if (stops.empty()) {
        return "No bus";
    }
    std::ostringstream ss;
    bool first = true;
    for (const auto& s : stops) {
        if (!first) {
            ss << std::endl;
        }
        first = false;
        ss << "Stop " << s << ": ";
        const auto& buses = catalog.busesForStop(s);
        if (buses.size() == 1) {
            ss << "no interchange";
        } else {
            for (const auto& b : buses) {
                if (b != bus) {
                    ss << b << " ";
                }
            }
        }
    }
    return ss.str();
}

std::string AllBuses(const BusRoutesCatalog& catalog) {
    const auto& all_buses = catalog.allBuses();
    if (all_buses.empty()) {
        return "No buses";
    }
    std::ostringstream ss;
    bool first = true;
    for (const auto& p : all_buses) {
        if (!first) {
            ss << std::endl;
        }
        first = false;
        ss << "Bus " << p.first << ": ";
        for (const auto& s : p.second) {
            ss << s << ' ';
        }
    }
    return ss.str();
}

int main() {
    int n;
    std::cin >> n;
    BusRoutesCatalog catalog;
    for (int i = 0; i < n; ++i) {
        std::string op, bus, stop;
        std::cin >> op;
        if (op == "NEW_BUS") {
            int stops_count;
            std::cin >> bus >> stops_count;
            std::vector<std::string> stops(stops_count);
            for (auto& s : stops) {
                std::cin >> s;
            }
            catalog.add(bus, stops);
        } else if (op == "BUSES_FOR_STOP") {
            std::cin >> stop;
            std::cout << BusesForStop(stop, catalog) << std::endl;
        } else if (op == "STOPS_FOR_BUS") {
            std::cin >> bus;
            std::cout << StopsForBus(bus, catalog) << std::endl;
        } else if (op == "ALL_BUSES") {
            std::cout << AllBuses(catalog) << std::endl;
        }
    }
    return 0;
}
