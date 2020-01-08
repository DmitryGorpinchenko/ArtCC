#include "responses.h"

std::ostream& operator <<(std::ostream& os, const BusesForStopResponse& r) {
    const auto& buses = r.buses;
    if (buses.empty()) {
        os << "No stop";
    } else {
        for (const auto& b : buses) {
            os << b << ' ';
        }
    }
    return os;
}

std::ostream& operator <<(std::ostream& os, const StopsForBusResponse& r) {
    const auto& stops = r.stops;
    if (stops.empty()) {
        os << "No bus";
    } else {
        bool first = true;
        for (const auto& [ stop, buses ] : stops) {
            if (!first) {
                os << std::endl;
            }
            first = false;
            os << "Stop " << stop << ": ";
            if (buses.empty()) {
                os << "no interchange";
            } else {
                for (const auto& b : buses) {
                    os << b << " ";
                }
            }
        }
    }
    return os;
}

std::ostream& operator <<(std::ostream& os, const AllBusesResponse& r) {
    const auto& all_buses = r.stops_for_bus;
    if (all_buses.empty()) {
        os << "No buses";
    } else {
        bool first = true;
        for (const auto& [ bus, stops ] : all_buses) {
            if (!first) {
                os << std::endl;
            }
            first = false;
            os << "Bus " << bus << ": ";
            for (const auto& s : stops) {
                os << s << " ";
            }
        }
    }
    return os;
}
