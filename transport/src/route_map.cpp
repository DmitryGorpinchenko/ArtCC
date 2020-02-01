#include "route_map.h"

#include <unordered_set>
using namespace std;

RouteMap::RouteMap(const Json::Document& doc) {
    const auto& doc_dict = doc.GetRoot().AsDict();
    for (const auto& n : doc_dict.at("base_requests").AsArray()) {
        const auto& dict = n.AsDict();
        const auto& type = dict.at("type").AsString();
        if (type == "Stop") {
            const auto& name = dict.at("name").AsString();
            const auto lat = dict.at("latitude").AsNumber();
            const auto lon = dict.at("longitude").AsNumber();
            unordered_map<string, double> dist_map;
            for (const auto& [k, v] : dict.at("road_distances").AsDict()) {
                dist_map.emplace(k, v.AsNumber());
            }
            AddStop(name, {lat, lon}, dist_map);
        } else if (type == "Bus") {
            const auto is_roubdtrip = dict.at("is_roundtrip").AsBool();
            const auto& name = dict.at("name").AsString();
            const auto& stops_arr = dict.at("stops").AsArray();
            vector<string> stops;
            stops.reserve(stops_arr.size());
            for (const auto& n : stops_arr) {
                stops.push_back(n.AsString());
            }
            AddBus(name, stops, is_roubdtrip);
        }
    }
    const auto& routing_dict = doc_dict.at("routing_settings").AsDict();
    router_data.wait_time = routing_dict.at("bus_wait_time").AsNumber();
    router_data.velocity = routing_dict.at("bus_velocity").AsNumber();
}

std::optional<Route> RouteMap::GetRoute(const std::string& from, const std::string& to) const {
    const auto& graph_data = GetGraphData(router_data);
    auto& router = GetRouter(router_data);
    const auto info = router.BuildRoute(graph_data.vertex_data.at(from).wait, graph_data.vertex_data.at(to).wait);
    if (!info) {
        return nullopt;
    }
    Route res;
    res.total_time = info->weight;
    for (size_t i = 0; i < info->edge_count; ++i) {
        res.items.push_back(graph_data.edge_data.at(router.GetRouteEdge(info->id, i)));
    }
    router.ReleaseRoute(info->id);
    return res;
}

optional<RouteInfo> RouteMap::GetRouteInfo(const string& bus) const {
    if (auto it = bus_data.find(bus); it != end(bus_data)) {
        const auto& data = (*it).second;
        const auto road_len = GetPartialRoadLen(data).back();
        const auto geo_len = GetGeoLen(data);
        return RouteInfo{
            .stops_num = data.stops_num,
            .unique_stops_num = data.unique_stops_num,
            .len = road_len,
            .curv = road_len / geo_len
        };
    }
    return nullopt;
}

optional<Buses> RouteMap::GetBusesFor(const string& stop) const {
    if (auto it = stop_data.find(stop); it != end(stop_data)) {
        const auto& bus_names = (*it).second.buses;
        return Buses{
            begin(bus_names),
            end(bus_names)
        };
    }
    return nullopt;
}

void RouteMap::AddStop(const string& stop, const Geo::Point& p, const unordered_map<string, double>& dist_map) {
    auto& data = stop_data[stop];
    data.coords = p;
    for (const auto& [k, v] : dist_map) {
        data.dist[k] = v;
        stop_data[k].dist.emplace(stop, v);
    }
}

void RouteMap::AddBus(const string& bus, const std::vector<std::string>& stop_names, bool is_roundtrip) {
    for (const auto& s : stop_names) {
        stop_data[s].buses.emplace(bus);
    }
    const auto stops_num = is_roundtrip ? stop_names.size() : (2 * stop_names.size() - 1);
    const auto unique_stops_num = unordered_set<string>(begin(stop_names), end(stop_names)).size();
    bus_data.emplace(bus, BusData{
        .stops = stop_names,
        .is_roundtrip = is_roundtrip,
        .stops_num = stops_num,
        .unique_stops_num = unique_stops_num
    });
}

double RouteMap::GeoDistance(const string& stop1, const string& stop2) const {
    return Geo::GreatCircleDistance(stop_data.at(stop1).coords, stop_data.at(stop2).coords);
}

double RouteMap::RoadDistance(const string& stop1, const string& stop2) const {
    return stop_data.at(stop1).dist.at(stop2);
}

const std::vector<double>& RouteMap::GetPartialRoadLen(const BusData& data) const {
    if (!data.partial_road_len) {
        vector<double> len;
        len.reserve(data.stops_num);
        len.push_back(0.);
        for (auto it = next(begin(data.stops)); it != end(data.stops); ++it) {
            len.push_back(len.back() + RoadDistance(*prev(it), *it));
        }
        if (!data.is_roundtrip) {
            for (auto it = next(rbegin(data.stops)); it != rend(data.stops); ++it) {
                len.push_back(len.back() + RoadDistance(*prev(it), *it));
            }
        }
        data.partial_road_len = move(len);
    }
    return *data.partial_road_len;
}

double RouteMap::GetGeoLen(const BusData& data) const {
    if (!data.geo_len) {
        double dist = 0.;
        for (auto it = next(begin(data.stops)); it != end(data.stops); ++it) {
            dist += GeoDistance(*prev(it), *it);
        }
        data.geo_len = dist * (1 + (!data.is_roundtrip));
    }
    return *data.geo_len;
}

const RouteMap::RouterData::GraphData& RouteMap::GetGraphData(const RouterData& data) const {
    if (!data.graph_data) {
        auto& graph_data = data.graph_data.emplace(2 * stop_data.size());
        auto& vertex_data = graph_data.vertex_data;
        auto& edge_data = graph_data.edge_data;
        auto& graph = graph_data.graph;
        Graph::VertexId next_id = 0;
        for (const auto& [k, v] : stop_data) {
            RouterData::GraphData::Vertices vs = {next_id, next_id + 1};
            vertex_data.emplace(k, vs);
            edge_data.emplace(graph.AddEdge({vs.wait, vs.bus, data.wait_time}),
                              Route::Element{Route::Element::Type::WAIT, k, data.wait_time});
            next_id += 2;
        }
        static const auto time_min = [](double dist_m, double velocity_km_h) {
            static const double km_h_to_m_min = 50./3.;
            return dist_m / (km_h_to_m_min * velocity_km_h);
        };
        for (const auto& [k, v] : bus_data) {
            const auto& stops = v.stops;
            const auto& partial_len = GetPartialRoadLen(v);
            for (size_t i = 0; i < stops.size(); ++i) {
                for (size_t j = i + 1; j < stops.size(); ++j) {
                    const auto& i_v = vertex_data.at(stops[i]);
                    const auto& j_v = vertex_data.at(stops[j]);
                    const auto span_count = j - i;
                    const auto time = time_min(partial_len[j] - partial_len[i], data.velocity);
                    edge_data.emplace(graph.AddEdge({i_v.bus, j_v.wait, time}),
                                      Route::Element{Route::Element::Type::BUS, Route::Element::Bus{k, span_count}, time});
                }
            }
            if (!v.is_roundtrip) {
                for (size_t i = stops.size(); i > 0; --i) {
                    for (size_t j = i - 1; j > 0; --j) {
                        const size_t ii = i - 1;
                        const size_t jj = j - 1;
                        const auto& ii_v = vertex_data.at(stops[ii]);
                        const auto& jj_v = vertex_data.at(stops[jj]);
                        const auto span_count = ii - jj;
                        const size_t ii_len = stops.size() + stops.size() - i - 1;
                        const size_t jj_len = stops.size() + stops.size() - j - 1;
                        const auto time = time_min(partial_len[jj_len] - partial_len[ii_len], data.velocity);
                        edge_data.emplace(graph.AddEdge({ii_v.bus, jj_v.wait, time}),
                                          Route::Element{Route::Element::Type::BUS, Route::Element::Bus{k, span_count}, time});
                    }
                }
            }
        }
    }
    return *data.graph_data;
}

Graph::Router<double>& RouteMap::GetRouter(const RouterData& data) const {
    if (!data.router) {
        data.router.emplace(GetGraphData(data).graph);
    }
    return *data.router;
}

