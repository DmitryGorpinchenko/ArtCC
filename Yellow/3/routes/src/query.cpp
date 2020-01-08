#include "query.h"

std::istream& operator >>(std::istream& is, Query& q) {
    std::string op;
    is >> op;
    if (op == "NEW_BUS") {
        q.type = QueryType::NewBus;
        int stops_count;
        is >> q.bus >> stops_count;
        q.stops.resize(stops_count);
        for (auto& s : q.stops) {
            is >> s;
        }
    } else if (op == "BUSES_FOR_STOP") {
        q.type = QueryType::BusesForStop;
        is >> q.stop;
    } else if (op == "STOPS_FOR_BUS") {
        q.type = QueryType::StopsForBus;
        is >> q.bus;
    } else if (op == "ALL_BUSES") {
        q.type = QueryType::AllBuses;
    }
    return is;
}
