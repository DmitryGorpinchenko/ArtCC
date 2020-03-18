#include "database.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <cstdint>
using namespace std;

namespace {

Request ToRequest(const Json::Dict& dict) {
    const auto id = int(dict.at("id").AsNumber());
    const auto type = dict.at("type").AsString();
    if (type == "Bus") {
        return Request(id, Request::Bus{
            dict.at("name").AsString()
        });
    } else if (type == "Stop") {
        return Request(id, Request::Stop{
            dict.at("name").AsString()
        });
    } else if (type == "Route") {
        return Request(id, Request::Route{
            dict.at("from").AsString(),
            dict.at("to").AsString()
        });
    }
    return Request(id, Request::Map{});
}

Json::Dict ToDict(const BusRoute::Element::Wait& item) {
    return {
        {"type", Json::String("Wait")},
        {"stop_name", item.stop}
    };
}

Json::Dict ToDict(const BusRoute::Element::Bus& item) {
    return {
        {"type", Json::String("Bus")},
        {"bus", item.name},
        {"span_count", Json::Number(item.span)}
    };
}

Json::Dict ToDict(const BusRoute::Element& item) {
    auto res = std::visit([](const auto& data) { return ToDict(data); }, item.data);
    res.emplace("time", Json::Number(item.time));
    return res;
}

Json::Dict ToDict(const Response::Bus& resp) {
    return {
        {"route_length", Json::Number(resp.len)},
        {"curvature", Json::Number(resp.curv)},
        {"stop_count", Json::Number(resp.stops_num)},
        {"unique_stop_count", Json::Number(resp.unique_stops_num)}
    };
}

Json::Dict ToDict(const Response::Stop& resp) {
    Json::Array buses;
    for (const auto& b : resp) {
        buses.emplace_back(b);
    }
    return {
        {"buses", move(buses)}
    };
}

Json::Dict ToDict(const Response::Route& resp) {
    Json::Array items;
    for (const auto& i : resp.data.items) {
        items.emplace_back(ToDict(i));
    }
    ostringstream ss;
    resp.img.Render(ss);
    return {
        {"total_time", Json::Number(resp.data.total_time)},
        {"items", move(items)},
        {"map", ss.str()}
    };
}

Json::Dict ToDict(const Response::Map& resp) {
    ostringstream ss;
    resp.Render(ss);
    return {
        {"map", ss.str() }
    };
}

Json::Dict ToDict(const Response::NotFound& resp) {
    return {
        {"error_message", Json::String("not found")}
    };
}

Json::Dict ToDict(const Response& resp) {
    auto res = std::visit([](const auto& resp) { return ToDict(resp); }, resp.Data());
    res.emplace("request_id", Json::Number(resp.Id()));
    return res;
}

} // end namespace

// Database

Json::Document ProcessRequests(const Database& db, const Json::Document& doc) {
    Json::Array array;
    for (const auto& n : doc.GetRoot().AsArray()) {
        array.push_back(ToDict(db.Process(ToRequest(n.AsDict()))));
    }
    return Json::Document(move(array));
}

