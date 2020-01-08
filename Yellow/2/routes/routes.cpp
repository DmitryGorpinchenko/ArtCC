#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include <utility>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator >>(istream& is, Query& q) {
    string op;
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

struct BusesForStopResponse {
    vector<string> buses;
};

ostream& operator <<(ostream& os, const BusesForStopResponse& r) {
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

struct StopsForBusResponse {
    vector<pair<string, vector<string>>> stops;
};

ostream& operator <<(ostream& os, const StopsForBusResponse& r) {
    const auto& stops = r.stops;
    if (stops.empty()) {
        os << "No bus";
    } else {
        bool first = true;
        for (const auto& [ stop, buses ] : stops) {
            if (!first) {
                os << endl;
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

struct AllBusesResponse {
    map<string, vector<string>> stops_for_bus;
};

ostream& operator <<(ostream& os, const AllBusesResponse& r) {
    const auto& all_buses = r.stops_for_bus;
    if (all_buses.empty()) {
        os << "No buses";
    } else {
        bool first = true;
        for (const auto& [ bus, stops ] : all_buses) {
            if (!first) {
                os << endl;
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

class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        if (stops_for_bus.emplace(bus, stops).second) {
            for (const auto& s : stops) {
                buses_for_stop[s].push_back(bus);
            }
        }
    }
    BusesForStopResponse GetBusesForStop(const string& stop) const {
        BusesForStopResponse res;
        auto it = buses_for_stop.find(stop);
        if (it != end(buses_for_stop)) {
            res.buses = (*it).second;
        }
        return res;
    }
    StopsForBusResponse GetStopsForBus(const string& bus) const {
        StopsForBusResponse res;
        auto it = stops_for_bus.find(bus);
        if (it != end(stops_for_bus)) {
            const auto& stops = (*it).second;
            res.stops.reserve(stops.size());
            for (const auto& s : stops) {
                const auto& buses = buses_for_stop.at(s);
                vector<string> interchanges;
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
    AllBusesResponse GetAllBuses() const {
        return { stops_for_bus };
    }
private:
    map<string, vector<string>> stops_for_bus;
    map<string, vector<string>> buses_for_stop;
};

int main() {
    int query_count;
    cin >> query_count;
    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        Query q;
        cin >> q;
        switch (q.type) {
        case QueryType::NewBus:
            bm.AddBus(q.bus, q.stops);
            break;
        case QueryType::BusesForStop:
            cout << bm.GetBusesForStop(q.stop) << endl;
            break;
        case QueryType::StopsForBus:
            cout << bm.GetStopsForBus(q.bus) << endl;
            break;
        case QueryType::AllBuses:
            cout << bm.GetAllBuses() << endl;
            break;
        }
    }
    return 0;
}
