#include "route_map.h"

using namespace std;

RouteMap::Router::Router(const RouteMap& _map, const RoutingSettings& _setts)
    : map(_map)
    , setts(_setts)
{}

optional<BusRoute> RouteMap::Router::BuildRoute(const string& from, const string& to) const {
    const auto& graph_data = GetGraphData();
    auto& router = GetRouter();
    const auto info = router.BuildRoute(graph_data.vertex_data.at(from).wait, graph_data.vertex_data.at(to).wait);
    if (!info) {
        return nullopt;
    }
    BusRoute res;
    for (size_t i = 0; i < info->edge_count; ++i) {
        res.items.push_back(graph_data.edge_data[router.GetRouteEdge(info->id, i)]);
    }
    res.total_time = info->weight;
    router.ReleaseRoute(info->id);
    return res;
}

const RouteMap::Router::GraphData& RouteMap::Router::GetGraphData() const {
    if (!graph_data) {
        auto& data = graph_data.emplace(2 * map.stop_data.size());
        Graph::VertexId next_id = 0;
        for (const auto& [k, _] : map.stop_data) {
            GraphData::Vertices vs = {next_id, next_id + 1};
            data.vertex_data.emplace(k, vs);
            data.graph.AddEdge({vs.wait, vs.bus, setts.wait_time});
            data.edge_data.push_back({BusRoute::Element::Wait{k}, setts.wait_time});
            next_id += 2;
        }
        using Adj = unordered_map<Graph::VertexId, optional<double>>;
        vector<Adj> weights(data.graph.GetVertexCount());
        const auto velocity_m_min = 50./3. * setts.velocity;
        const auto fillGraph = [&weights, &data, velocity_m_min](auto stop_first, auto stop_last, bool rev, auto len_first, const string& bus) {
            const auto count = size_t(distance(stop_first, stop_last));
            for (size_t i = 0; i < count; ++i) {
                for (size_t j = i + 1; j < count; ++j) {
                    const auto& i_v = data.vertex_data.at(*next(stop_first, i));
                    const auto& j_v = data.vertex_data.at(*next(stop_first, j));
                    const auto span = j - i;
                    const auto time = (*next(len_first, j) - *next(len_first, i)) / velocity_m_min;
                    auto& w = weights[i_v.bus][j_v.wait];
                    if ((!w) || (time < w)) {
                        data.graph.AddEdge({i_v.bus, j_v.wait, time});
                        data.edge_data.push_back({
                            BusRoute::Element::Bus{
                                .name = bus,
                                .from = i,
                                .span = span,
                                .rev = rev
                            },
                            time
                        });
                        w = time;
                    }
                }
            }
        };
        for (const auto& [k, v] : map.bus_data) {
            const auto& stops = v.stops;
            const auto& partial_len = map.GetPartialRoadLen(v);
            fillGraph(begin(stops), end(stops), false, begin(partial_len), k);
            if (!v.is_roundtrip) {
                fillGraph(rbegin(stops), rend(stops), true, next(begin(partial_len), stops.size() - 1), k);
            }
        }
    }
    return *graph_data;
}

Graph::Router<double>& RouteMap::Router::GetRouter() const {
    if (!router) {
        router.emplace(GetGraphData().graph);
    }
    return *router;
}
