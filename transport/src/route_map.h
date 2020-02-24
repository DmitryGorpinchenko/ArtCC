#pragma once

#include "geo.h"
#include "json.h"
#include "range.h"
#include "graph.h"
#include "svg.h"

#include <vector>
#include <string>
#include <optional>
#include <variant>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <utility>
#include <cstdint>

struct RouteInfo {
    size_t stops_num = 0;
    size_t unique_stops_num = 0;
    double len = 0;
    double curv = 0;
};

using Buses = Range<std::set<std::string>::iterator>;

struct BusRoute {
    struct Element {
        struct Wait {
            std::string stop;
        };
        struct Bus {
            std::string name;
            size_t from = 0;
            size_t span = 0;
            bool rev = false;
        };
        
        std::variant<Wait, Bus> data;
        double time = 0;
    };
    
    std::vector<Element> items;
    double total_time = 0;
};

struct RoutingSettings {
    double wait_time = 0.;
    double velocity = 0.;
};

namespace Layers {

struct BusLines {};
struct BusLabels {};
struct StopPoints {};
struct StopLabels {};

}
using Layer = std::variant<Layers::BusLines,
                           Layers::BusLabels,
                           Layers::StopPoints,
                           Layers::StopLabels>;

struct RenderSettings {
    double width;
    double height;
    double padding;
    double stop_radius;
    double line_width;
    double underlayer_width;
    double outer_margin;
    
    uint32_t bus_label_font_size;
    uint32_t stop_label_font_size;
    Svg::Point bus_label_offset;
    Svg::Point stop_label_offset;
    Svg::Color underlayer_color;
    std::vector<Svg::Color> color_palette;
    std::vector<Layer> layers;
};
using Image = Svg::Document;

class RouteMap {
    // Router
    class Router {
    public:
        Router(const RouteMap& _map, const RoutingSettings& _setts);
        
        std::optional<BusRoute> BuildRoute(const std::string& from, const std::string& to) const;
        
    private:
        struct GraphData;
        const GraphData& GetGraphData() const;
        Graph::Router<double>& GetRouter() const;
    
        const RouteMap& map;
        RoutingSettings setts;
        
        struct GraphData {
            struct Vertices {
                Graph::VertexId wait = 0;
                Graph::VertexId bus = 0;
            };
            std::unordered_map<std::string, Vertices> vertex_data;
            std::vector<BusRoute::Element> edge_data;
            Graph::DirectedWeightedGraph<double> graph;
            
            GraphData(size_t vertex_count) : graph(vertex_count) {}
        };
        mutable std::optional<GraphData> graph_data;
        mutable std::optional<Graph::Router<double>> router;
    };
    // Renderer
    class Renderer {
    public:
        Renderer(const RouteMap& _map, const RenderSettings& _setts);
        
        Image Render() const;
        Image Render(const BusRoute& route) const;
        
    private:
        void Add(Layers::BusLines, Image& img) const;
        void Add(Layers::BusLines, const BusRoute& route, Image& img) const;
        
        void Add(Layers::BusLabels, Image& img) const;
        void Add(Layers::BusLabels, const BusRoute& route, Image& img) const;
        
        void Add(Layers::StopPoints, Image& img) const;
        void Add(Layers::StopPoints, const BusRoute& route, Image& img) const;
        
        void Add(Layers::StopLabels, Image& img) const;
        void Add(Layers::StopLabels, const BusRoute& route, Image& img) const;
    
        struct Data;
        const Data& GetData() const;
    
        std::map<std::string, Svg::Point> ComputeStopCoords() const;
        std::map<std::string, Svg::Color> ComputeBusColors() const;
        
        using Stop = std::pair<std::string, Svg::Point>;
        void Interpolate(std::vector<Stop>& stops) const;
        void Project(std::vector<Stop>& stops) const;
        std::vector<Stop> Stops() const;
        std::unordered_set<std::string> RefStops() const;
        std::unordered_map<std::string, std::unordered_set<std::string>> AdjStops() const;
    
        const RouteMap& map;
        RenderSettings setts;
        
        struct Data {
            std::map<std::string, Svg::Point> stops;
            std::map<std::string, Svg::Color> buses;
        };
        mutable std::optional<Data> data;
        mutable std::optional<Image> img;
    };
public:
    RouteMap(const Json::Document& doc);
    
    std::optional<BusRoute> GetRoute(const std::string& from, const std::string& to) const;
    std::optional<RouteInfo> GetRouteInfo(const std::string& bus) const;
    std::optional<Buses> GetBusesFor(const std::string& stop) const;
    
    Image Render() const;
    Image Render(const BusRoute& route) const;
    
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
    
    Router router;
    Renderer renderer;
};
