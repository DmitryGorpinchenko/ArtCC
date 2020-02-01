#include "database.h"

#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

Database::Database(const Json::Document& doc)
    : map(doc)
{}

//

Request::Type RequestType(const std::string& type) {
    if (type == "Bus") {
        return Request::Type::BUS;
    } else if (type == "Stop") {
        return Request::Type::STOP;
    } else if (type == "Route") {
        return Request::Type::ROUTE;
    }
    return {};
}

Request ToRequest(const Json::Dict& dict) {
    Request res;
    res.id = dict.at("id").AsNumber();
    res.type = RequestType(dict.at("type").AsString());
    switch (res.type) {
    case Request::Type::ROUTE:
        res.data = Endpoints{
            dict.at("from").AsString(),
            dict.at("to").AsString()
        };
        break;
    case Request::Type::STOP:
    case Request::Type::BUS:
        res.data = dict.at("name").AsString();
        break;
    }
    return res;
}

Json::Dict ToDict(const Route::Element& item) {
    Json::Dict res;
    if (item.type == Route::Element::Type::WAIT) {
        res.emplace("type", Json::String("Wait"));
        res.emplace("stop_name", item.Data<std::string>());
    } else if (item.type == Route::Element::Type::BUS) {
        const auto& bus = item.Data<Route::Element::Bus>();
        res.emplace("type", Json::String("Bus"));
        res.emplace("bus", bus.name);
        res.emplace("span_count", Json::Number(bus.span_count));
    }
    res.emplace("time", Json::Number(item.time));
    return res;
}

Json::Dict ToDict(const Response& resp) {
    Json::Dict res;
    res.emplace("request_id", Json::Number(resp.GetId()));
    if (resp.Has<RouteInfo>()) {
        const auto& info = resp.Get<RouteInfo>();
        res.emplace("route_length", Json::Number(info.len));
        res.emplace("curvature", Json::Number(info.curv));
        res.emplace("stop_count", Json::Number(info.stops_num));
        res.emplace("unique_stop_count", Json::Number(info.unique_stops_num));
    } else if (resp.Has<Buses>()) {
        Json::Array buses;
        for (const auto& b : resp.Get<Buses>()) {
            buses.emplace_back(b);
        }
        res.emplace("buses", move(buses));
    } else if (resp.Has<Route>()) {
        const auto& route = resp.Get<Route>();
        Json::Array items;
        for (const auto& i : route.items) {
            items.emplace_back(ToDict(i));
        }
        res.emplace("total_time", Json::Number(route.total_time));
        res.emplace("items", move(items));
    } else {
        res.emplace("error_message", Json::String("not found"));
    }
    return res; 
}

Json::Document ProcessRequests(const Database& db, const Json::Document& doc) {
    Json::Array array;
    for (const auto& n : doc.GetRoot().AsArray()) {
        array.push_back(ToDict(db.Process(ToRequest(n.AsDict()))));
    }
    return Json::Document(move(array));
}

