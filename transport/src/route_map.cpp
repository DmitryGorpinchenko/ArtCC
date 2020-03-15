#include "route_map.h"
#include "transport_catalog.pb.h"

#include <iterator>
using namespace std;

namespace {

template <typename T>
Svg::Color ToColor(const T& val) {
    return {};
}

Svg::Color ToColor(const Json::Array& arr) {
    const auto r = uint8_t(arr[0].AsNumber());
    const auto g = uint8_t(arr[1].AsNumber());
    const auto b = uint8_t(arr[2].AsNumber());
    std::optional<double> a;
    if (arr.size() == 4) {
        a = arr[3].AsNumber();
    }
    return Svg::Rgb{r, g, b, a};
}

Svg::Color ToColor(const Json::String& str) {
    return str;
}

Svg::Color ToColor(const Json::Node& node) {
    return node.Visit([](const auto& val) { return ToColor(val); });
}

RoutingSettings ExtractRoutingSettings(const Json::Document& doc) {
    const auto& dict = doc.GetRoot().AsDict().at("routing_settings").AsDict();
    return {
        .wait_time = dict.at("bus_wait_time").AsNumber(),
        .velocity = dict.at("bus_velocity").AsNumber(),
    };
}

RenderSettings ExtractRenderSettings(const Json::Document& doc) {
    const auto& dict = doc.GetRoot().AsDict().at("render_settings").AsDict();
    RenderSettings res;
    res.width = dict.at("width").AsNumber();
    res.height = dict.at("height").AsNumber();
    res.padding = dict.at("padding").AsNumber();
    res.stop_radius = dict.at("stop_radius").AsNumber();
    res.line_width = dict.at("line_width").AsNumber();
    res.underlayer_width = dict.at("underlayer_width").AsNumber();
    res.outer_margin = dict.at("outer_margin").AsNumber();
    res.bus_label_font_size = uint32_t(dict.at("bus_label_font_size").AsNumber());
    res.stop_label_font_size = uint32_t(dict.at("stop_label_font_size").AsNumber());

    const auto& bus_offset_arr = dict.at("bus_label_offset").AsArray();
    res.bus_label_offset = {bus_offset_arr[0].AsNumber(), bus_offset_arr[1].AsNumber()};
    const auto& stop_offset_arr = dict.at("stop_label_offset").AsArray();
    res.stop_label_offset = {stop_offset_arr[0].AsNumber(), stop_offset_arr[1].AsNumber()};

    res.underlayer_color = ToColor(dict.at("underlayer_color"));
    for (const auto& color : dict.at("color_palette").AsArray()) {
        res.color_palette.push_back(ToColor(color));
    }
    unordered_map<string_view, Layer> layers {
        {"bus_lines",   Layers::BusLines{}},
        {"bus_labels",  Layers::BusLabels{}},
        {"stop_points", Layers::StopPoints{}},
        {"stop_labels", Layers::StopLabels{}}
    };
    for (const auto& l : dict.at("layers").AsArray()) {
        res.layers.push_back(layers.at(l.AsString()));
    }
    return res;
}

}

RouteMap::RouteMap(const Json::Document& doc)
    : router(*this, ExtractRoutingSettings(doc))
    , renderer(*this, ExtractRenderSettings(doc))
{
    const auto& doc_dict = doc.GetRoot().AsDict();
    for (const auto& n : doc_dict.at("base_requests").AsArray()) {
        const auto& dict = n.AsDict();
        const auto& type = dict.at("type").AsString();
        if (type == "Stop") {
            const auto& name = dict.at("name").AsString();
            const auto lat = dict.at("latitude").AsNumber();
            const auto lon = dict.at("longitude").AsNumber();
            unordered_map<string, uint32_t> dist_map;
            for (const auto& [k, v] : dict.at("road_distances").AsDict()) {
                dist_map.emplace(k, v.AsNumber());
            }
            AddStop(name, {lat, lon}, dist_map);
        } else if (type == "Bus") {
            const auto is_roundtrip = dict.at("is_roundtrip").AsBool();
            const auto& name = dict.at("name").AsString();
            const auto& stops_arr = dict.at("stops").AsArray();
            vector<string> stops;
            stops.reserve(stops_arr.size());
            for (const auto& n : stops_arr) {
                stops.push_back(n.AsString());
            }
            AddBus(name, stops, is_roundtrip);
        }
    }
    for (auto& [bus, data] : bus_data) {
        data.geo_len = GeoLen(data.stops, data.is_roundtrip);
        data.road_len = RoadLen(data.stops, data.is_roundtrip);
    }
}

RouteMap::RouteMap(std::istream& is)
    : router(*this, {})
    , renderer(*this, {})
{
    RouteMapSerialize::TransportCatalog cat;
    cat.ParseFromIstream(&is);
    for (auto& s : *cat.mutable_stops()) {
        auto& name = *s.mutable_name();
        auto& buses = *s.mutable_buses();
        stop_data.emplace(
            move(name),
            StopData{
                {},
                {
                    make_move_iterator(begin(buses)),
                    make_move_iterator(end(buses))
                },
                {}
            }
        );
    }
    for (auto& b : *cat.mutable_buses()) {
        auto& name = *b.mutable_name();
        bus_data.emplace(
            move(name),
            BusData{
                {},
                {},
                b.stops_num(),
                b.unique_stops_num(),
                b.road_len(),
                b.geo_len()
            }
        );
    }
}

optional<BusRoute> RouteMap::GetRoute(const string& from, const string& to) const {
    return router.BuildRoute(from, to);
}

optional<RouteInfo> RouteMap::GetRouteInfo(const string& bus) const {
    if (auto it = bus_data.find(bus); it != end(bus_data)) {
        const auto& data = (*it).second;
        return RouteInfo{
            .stops_num = data.stops_num,
            .unique_stops_num = data.unique_stops_num,
            .len = data.road_len,
            .curv = data.road_len / data.geo_len
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

Image RouteMap::Render() const {
    return renderer.Render();
}

Image RouteMap::Render(const BusRoute& route) const {
    return renderer.Render(route);
}

void RouteMap::Serialize(std::ostream& os) {
    RouteMapSerialize::TransportCatalog cat;
    for (const auto& [s, data] : stop_data) {
        RouteMapSerialize::Stop stop;
        stop.set_name(s);
        for (const auto& b : data.buses) {
            *stop.add_buses() = b;
        }
        *cat.add_stops() = move(stop);
    }
    for (const auto& [b, data] : bus_data) {
        RouteMapSerialize::Bus bus;
        bus.set_name(b);
        bus.set_stops_num(data.stops_num);
        bus.set_unique_stops_num(data.unique_stops_num);
        bus.set_road_len(data.road_len);
        bus.set_geo_len(data.geo_len);
        *cat.add_buses() = move(bus);
    }
    cat.SerializeToOstream(&os);
}

void RouteMap::AddStop(const string& stop, const Geo::Point& p, const unordered_map<string, uint32_t>& dist_map) {
    auto& data = stop_data[stop];
    data.coords = p;
    for (const auto& [k, v] : dist_map) {
        data.dist[k] = v;
        stop_data[k].dist.emplace(stop, v);
    }
}

void RouteMap::AddBus(const string& bus, const vector<string>& stop_names, bool is_roundtrip) {
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

double RouteMap::GeoLen(const vector<string>& stops, bool is_roundtrip) const {
    double dist = 0.;
    for (auto it = next(begin(stops)); it != end(stops); ++it) {
        dist += GeoDistance(*prev(it), *it);
    }
    return dist * (1 + (!is_roundtrip));
}

uint32_t RouteMap::RoadLen(const vector<string>& stops, bool is_roundtrip) const {
    uint32_t dist = 0;
    for (auto it = next(begin(stops)); it != end(stops); ++it) {
        dist += RoadDistance(*prev(it), *it);
    }
    if (!is_roundtrip) {
        for (auto it = next(rbegin(stops)); it != rend(stops); ++it) {
            dist += RoadDistance(*prev(it), *it);
        }
    }
    return dist;
}

double RouteMap::GeoDistance(const string& stop1, const string& stop2) const {
    return Geo::GreatCircleDistance(stop_data.at(stop1).coords, stop_data.at(stop2).coords);
}

uint32_t RouteMap::RoadDistance(const string& stop1, const string& stop2) const {
    return stop_data.at(stop1).dist.at(stop2);
}

