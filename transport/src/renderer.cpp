#include "route_map.h"

#include <algorithm>
#include <optional>
#include <vector>
#include <iterator>
using namespace std;

namespace {

void AddPolyline(vector<Svg::Point> points, const Svg::Color& c, double w, Image& img) {
    Svg::Polyline l;
    l.SetStrokeColor(c)
     .SetStrokeWidth(w)
     .SetStrokeLineCap("round")
     .SetStrokeLineJoin("round");
    l.AddPoints(move(points));
    img.Add(move(l));
}

void AddLabel(const string& text, Svg::Point p, Svg::Point offset, const Svg::Color& c, uint32_t font_size, bool bold, const RenderSettings& setts, Image& img) {
    Svg::Text substrate, name;
    substrate.SetFillColor(setts.underlayer_color)
         .SetStrokeColor(setts.underlayer_color)
         .SetStrokeWidth(setts.underlayer_width)
         .SetStrokeLineCap("round")
         .SetStrokeLineJoin("round")
         .SetFontFamily("Verdana")
         .SetFontSize(font_size)
         .SetPoint(p)
         .SetOffset(offset)
         .SetData(text);
    name.SetFillColor(c)
        .SetFontFamily("Verdana")
        .SetFontSize(font_size)
        .SetPoint(p)
        .SetOffset(offset)
        .SetData(text);
    if (bold) {
        substrate.SetFontWeight("bold");
        name.SetFontWeight("bold");
    }
    img.Add(move(substrate));
    img.Add(move(name));
}

void AddRect(const RenderSettings& setts, Image& img) {
    Svg::Rect r;
    r.SetFillColor(setts.underlayer_color)
     .SetTopLeft({-setts.outer_margin, -setts.outer_margin})
     .SetWidth(setts.width + 2 * setts.outer_margin)
     .SetHeight(setts.height + 2 * setts.outer_margin);
    img.Add(move(r));
}

void AddCircle(Svg::Point center, double r, Image& img) {
    Svg::Circle c;
    c.SetFillColor("white")
     .SetCenter(center)
     .SetRadius(r);
    img.Add(move(c));
}

}

RouteMap::Renderer::Renderer(const RouteMap& _map, const RenderSettings& _setts)
    : map(_map)
    , setts(_setts)
{}

Image RouteMap::Renderer::Render() const {
    if (!img) {
        img.emplace();
        for (const auto& l : setts.layers) {
            std::visit([this](const auto& val) { Add(val, *img); }, l);
        }
    }
    return *img;
}

Image RouteMap::Renderer::Render(const BusRoute& route) const {
    auto res = Render();
    AddRect(setts, res);
    for (const auto& l : setts.layers) {
        std::visit([this, &res, &route](const auto& val) { Add(val, route, res); }, l);
    }
    return res;
}

// BusLines

void RouteMap::Renderer::Add(Layers::BusLines, Image& img) const {
    const auto& stops = GetData().stops;
    for (const auto& [b, c] : GetData().buses) {
        const auto& data = map.bus_data.at(b);
        vector<Svg::Point> points;
        points.reserve(data.stops_num);
        for (const auto& s : data.stops) {
            points.push_back(stops.at(s));
        }
        if (!data.is_roundtrip) {
            for (const auto& s : Range(next(rbegin(data.stops)), rend(data.stops))) {
                points.push_back(stops.at(s));
            }
        }
        AddPolyline(move(points), c, setts.line_width, img);
    }
}

void RouteMap::Renderer::Add(Layers::BusLines, const BusRoute& route, Image& img) const {
    const auto& buses = GetData().buses;
    const auto points = [this](auto first, size_t span) {
        vector<Svg::Point> res;
        res.reserve(span + 1);
        const auto& stops = GetData().stops;
        for (const auto& s : Range(first, first + span + 1)) {
            res.push_back(stops.at(s));
        }
        return res;
    };
    for (const auto& i : route.items) {
        if (auto pi = get_if<BusRoute::Element::Bus>(&i.data)) {
            const auto& [name, from, span, rev] = *pi;
            const auto& stops = map.bus_data.at(name).stops;
            AddPolyline(rev ? points(rbegin(stops) + from, span) : points(begin(stops) + from, span),
                        buses.at(name),
                        setts.line_width,
                        img);
        }
    }
}

// BusLabels

void RouteMap::Renderer::Add(Layers::BusLabels, Image& img) const {
    const auto& stops = GetData().stops;
    for (const auto& [b, c] : GetData().buses) {
        const auto& data = map.bus_data.at(b);
        const auto endpoints = (data.stops.front() == data.stops.back())
                                ? vector<string>{data.stops.front()}
                                : vector<string>{data.stops.front(), data.stops.back()};
        for (const auto& e : endpoints) {
            AddLabel(b, stops.at(e), setts.bus_label_offset, c, setts.bus_label_font_size, true, setts, img);
        }
    }
}

void RouteMap::Renderer::Add(Layers::BusLabels, const BusRoute& route, Image& img) const {
    const auto& stops = GetData().stops;
    const auto& buses = GetData().buses;
    for (const auto& i : route.items) {
        if (auto pi = get_if<BusRoute::Element::Bus>(&i.data)) {
            const auto& [name, from, span, rev] = *pi;
            const auto& data = map.bus_data.at(name);
            const size_t l = from;
            const size_t r = l + span;
            auto ends = rev ? vector<string>{*(rbegin(data.stops) + l), *(rbegin(data.stops) + r)}
                            : vector<string>{*(begin(data.stops) + l), *(begin(data.stops) + r)};
            auto new_end = remove_if(begin(ends), end(ends), [&data](const string& stop) {
                return (stop != data.stops.front()) && (stop != data.stops.back());
            });
            for (const auto& e : Range(begin(ends), new_end)) {
                AddLabel(name, stops.at(e), setts.bus_label_offset, buses.at(name), setts.bus_label_font_size, true, setts, img);
            }
        }
    }
}

// StopPoints

void RouteMap::Renderer::Add(Layers::StopPoints, Image& img) const {
    for (const auto& [_, p] : GetData().stops) {
        AddCircle(p, setts.stop_radius, img);
    }
}

void RouteMap::Renderer::Add(Layers::StopPoints, const BusRoute& route, Image& img) const {
    const auto add_circles = [this, &img](auto first, size_t span) {
        const auto& stops = GetData().stops;
        for (const auto& s : Range(first, first + span + 1)) {
            AddCircle(stops.at(s), setts.stop_radius, img);
        }
    };
    for (const auto& i : route.items) {
        if (auto pi = get_if<BusRoute::Element::Bus>(&i.data)) {
            const auto& [name, from, span, rev] = *pi;
            const auto& data = map.bus_data.at(name);
            if (rev) {
                add_circles(rbegin(data.stops) + from, span);
            } else {
                add_circles(begin(data.stops) + from, span);
            }
        }
    }
}

// StopLabels

void RouteMap::Renderer::Add(Layers::StopLabels, Image& img) const {
    for (const auto& [s, p] : GetData().stops) {
        AddLabel(s, p, setts.stop_label_offset, "black", setts.stop_label_font_size, false, setts, img);
    }
}

void RouteMap::Renderer::Add(Layers::StopLabels, const BusRoute& route, Image& img) const {
    const auto& stops = GetData().stops;
    for (const auto& i : route.items) {
        if (auto pi = get_if<BusRoute::Element::Wait>(&i.data)) {
            const auto& s = pi->stop;
            const auto& p = stops.at(s);
            AddLabel(s, p, setts.stop_label_offset, "black", setts.stop_label_font_size, false, setts, img);
        }
    }
    if (!route.items.empty()) {
        const auto& [name, from, span, rev] = get<BusRoute::Element::Bus>(route.items.back().data);
        const auto& data = map.bus_data.at(name);
        const size_t r = from + span;
        const auto& last_stop = rev ? *(rbegin(data.stops) + r) : *(begin(data.stops) + r);
        AddLabel(last_stop, stops.at(last_stop), setts.stop_label_offset, "black", setts.stop_label_font_size, false, setts, img);
    }
}

//

const RouteMap::Renderer::Data& RouteMap::Renderer::GetData() const {
    if (!data) {
        data = {
            ComputeStopCoords(),
            ComputeBusColors()
        };
    }
    return *data;
}

map<string, Svg::Point> RouteMap::Renderer::ComputeStopCoords() const {
    auto stops = Stops();
    Interpolate(stops);
    Project(stops);
    return {
        make_move_iterator(begin(stops)),
        make_move_iterator(end(stops))
    };
}

map<string, Svg::Color> RouteMap::Renderer::ComputeBusColors() const {
    std::map<std::string, Svg::Color> res;
    for (const auto& [b, _] : map.bus_data) {
        res.emplace(b, Svg::Color());
    }
    size_t color_id = 0;
    for (auto& [b, c] : res) {
        c = setts.color_palette[color_id];
        color_id = (color_id + 1) % setts.color_palette.size();
    }
    return res;
}

void RouteMap::Renderer::Interpolate(vector<Stop>& stops) const {
    const auto ref_stops = RefStops();
    unordered_map<string, Svg::Point> intermediate_stops;
    for (const auto& [b, data] : map.bus_data) {
        auto l = begin(data.stops);
        while (l != end(data.stops)) {
            auto r = find_if(next(l),
                             end(data.stops),
                             [&ref_stops](const string& s) { return ref_stops.count(s); });
            if (r != end(data.stops)) {
                const auto& l_p = map.stop_data.at(*l).coords;
                const auto& r_p = map.stop_data.at(*r).coords;
                const double lon_step = (r_p.LonDeg() - l_p.LonDeg()) / (r - l);
                const double lat_step = (r_p.LatDeg() - l_p.LatDeg()) / (r - l);
                for (auto it = next(l); it != r; ++it) {
                    intermediate_stops[*it] = {
                        l_p.LonDeg() + lon_step * (it - l),
                        l_p.LatDeg() + lat_step * (it - l)
                    };
                }
            }
            l = r;
        }
    }
    for (auto& [s, p] : stops) {
        if (auto it = intermediate_stops.find(s); it != end(intermediate_stops)) {
            p = (*it).second;
        }
    }
}

void RouteMap::Renderer::Project(vector<Stop>& stops) const {
    const auto adj_stops = AdjStops();
    const auto calc_coord = [&adj_stops, &stops](auto coord, double size, double pad, bool inv) {
        sort(begin(stops),
             end(stops),
             [coord](const Stop& lhs, const Stop& rhs) {
                 return lhs.second.*coord < rhs.second.*coord;
             });
        vector<size_t> ids(stops.size(), 0);
        for (size_t i = 1; i < stops.size(); ++i) {
            const auto& adj = adj_stops.at(stops[i].first);
            for (size_t j = 0; j < i; ++j) {
                if (adj.count(stops[j].first)) {
                    if (ids[i] < ids[j] + 1) {
                        ids[i] = ids[j] + 1;
                    }
                }
            }
        }
        auto max_it = max_element(begin(ids), end(ids));
        const double step = (max_it != end(ids) && *max_it > 0) ? ((size - 2 * pad) / *max_it) : 0.;
        for (size_t i = 0; i < stops.size(); ++i) {
            const double val = ids[i] * step + pad;
            stops[i].second.*coord = inv ? (size - val) : val;
        }
    };
    calc_coord(&Svg::Point::x, setts.width, setts.padding, false);
    calc_coord(&Svg::Point::y, setts.height, setts.padding, true);
}

vector<RouteMap::Renderer::Stop> RouteMap::Renderer::Stops() const {
    vector<Stop> res;
    res.reserve(map.stop_data.size());
    for (const auto& [k, v] : map.stop_data) {
        res.emplace_back(k, Svg::Point{v.coords.LonDeg(), v.coords.LatDeg()});
    }
    return res;
}

unordered_set<string> RouteMap::Renderer::RefStops() const {
    unordered_set<string> res;
    for (const auto& [s, data] : map.stop_data) {
        if (data.buses.size() > 1) {
            res.insert(s);
        }
    }
    for (const auto& [b, data] : map.bus_data) {
        const auto& stops = data.stops;
        res.insert(stops.front());
        res.insert(stops.back());
        std::unordered_map<string, int> counts;
        for (size_t i = stops.size(); i > 2; --i) {
            ++counts[stops[i - 2]];
        }
        const int threshold = data.is_roundtrip ? 2 : 1;
        for (const auto& [s, c] : counts) {
            if (c > threshold) {
                res.insert(s);
            }
        }
    }
    return res;
}

unordered_map<string, unordered_set<string>> RouteMap::Renderer::AdjStops() const {
    unordered_map<string, unordered_set<string>> res;
    for (const auto& [k, v] : map.stop_data) {
        res.emplace(k, unordered_set<string>());
    }
    for (const auto& [b, data] : map.bus_data) {
        const auto& stops = data.stops;
        const size_t count = stops.size();
        for (size_t i = 0; i < count - 1; ++i) {
            res[stops[i]].insert(stops[i + 1]);
            res[stops[i + 1]].insert(stops[i]);
        }
    }
    return res;
}

