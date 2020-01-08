#include "bus_manager.h"

void BusManager::AddBus(const std::string& bus, const std::vector<std::string>& stops) {
    if (stops_for_bus.emplace(bus, stops).second) {
        for (const auto& s : stops) {
            buses_for_stop[s].push_back(bus);
        }
    }
}

BusesForStopResponse BusManager::GetBusesForStop(const std::string& stop) const {
    BusesForStopResponse res;
    auto it = buses_for_stop.find(stop);
    if (it != end(buses_for_stop)) {
        res.buses = (*it).second;
    }
    return res;
}

StopsForBusResponse BusManager::GetStopsForBus(const std::string& bus) const {
    StopsForBusResponse res;
    auto it = stops_for_bus.find(bus);
    if (it != end(stops_for_bus)) {
        const auto& stops = (*it).second;
        res.stops.reserve(stops.size());
        for (const auto& s : stops) {
            const auto& buses = buses_for_stop.at(s);
            std::vector<std::string> interchanges;
            interchanges.reserve(buses.size());
            for (const auto& b : buses) {
                if (b != bus) {
                    interchanges.push_back(b);
                }
            }
            res.stops.push_back({ s, interchanges });
        }
    }
    return res;
}

AllBusesResponse BusManager::GetAllBuses() const {
    return { stops_for_bus };
}
