#pragma once

#include "geo.h"
#include "json.h"
#include "range.h"
#include "graph.h"

#include <vector>
#include <string>
#include <optional>
#include <variant>
#include <unordered_map>
#include <set>

struct RouteInfo {
    size_t stops_num = 0;
    size_t unique_stops_num = 0;
    double len = 0;
    double curv = 0;
};
using Buses = Range<std::set<std::string>::iterator>;

struct Route {
    struct Element {
        enum class Type {
            WAIT,
            BUS,
        };
        struct Bus {
            std::string name;
            size_t span_count = 0;
        };
        
        template <typename T>
        const auto& Data() const { return std::get<T>(data); }
        
        Type type;
        std::variant<std::monostate, std::string, Bus> data;
        double time = 0;
    };
    
    double total_time = 0;
    std::vector<Element> items;
};

class RouteMap {
public:
    RouteMap(const Json::Document& doc);
    
    std::optional<Route> GetRoute(const std::string& from, const std::string& to) const;
    std::optional<RouteInfo> GetRouteInfo(const std::string& bus) const;
    std::optional<Buses> GetBusesFor(const std::string& stop) const;
    
private:
    void AddStop(const std::string& stop, const Geo::Point& p, const std::unordered_map<std::string, double>& dist_map);
    void AddBus(const std::string& bus, const std::vector<std::string>& stop_names, bool is_roundtrip);

    double GeoDistance(const std::string& stop1, const std::string& stop2) const;
    double RoadDistance(const std::string& stop1, const std::string& stop2) const;

    struct BusData;
    const std::vector<double>& GetPartialRoadLen(const BusData& data) const;
    double GetGeoLen(const BusData& data) const;

    struct BusData {
        std::vector<std::string> stops;
        bool is_roundtrip = false;
        size_t stops_num = 0;
        size_t unique_stops_num = 0;

        mutable std::optional<std::vector<double>> partial_road_len;
        mutable std::optional<double> geo_len;
    };
    std::unordered_map<std::string, BusData> bus_data;
    
    struct StopData {
        Geo::Point coords;
        std::set<std::string> buses;
        std::unordered_map<std::string, double> dist;
    };
    std::unordered_map<std::string, StopData> stop_data;
    
    struct RouterData {
        double wait_time = 0.;
        double velocity = 0.;
        
        struct GraphData {
            struct Vertices {
                Graph::VertexId wait = 0;
                Graph::VertexId bus = 0;
            };
            std::unordered_map<std::string, Vertices> vertex_data;
            std::unordered_map<Graph::EdgeId, Route::Element> edge_data;
            Graph::DirectedWeightedGraph<double> graph;
            
            GraphData(size_t vertex_count) : graph(vertex_count) {}
        };
        mutable std::optional<GraphData> graph_data;
        mutable std::optional<Graph::Router<double>> router;
    };
    RouterData router_data;
    
    const RouterData::GraphData& GetGraphData(const RouterData& data) const;
    Graph::Router<double>& GetRouter(const RouterData& data) const;
};
