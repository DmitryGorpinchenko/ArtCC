#pragma once

#include "route_map.h"
#include "json.h"

#include <optional>
#include <variant>

class Request {
public:
    struct Bus {
        std::string name;
    };
    struct Stop {
        std::string name;
    };
    struct Route {
        std::string from;
        std::string to;
    };
    struct Map {};
    
    template <typename T>
    explicit Request(int _id, T&& _data) : id(_id), data(std::forward<T>(_data)) {}
    
    int Id() const { return id; }
    const auto& Data() const { return data; }
    
private:
    int id = 0;
    std::variant<Bus, Stop, Route, Map> data;
};

class Response {
public:
    using Bus = RouteInfo;
    using Stop = Buses;
    struct Route {
        BusRoute data;
        Image img;
    };
    using Map = Image;
    using NotFound = std::monostate;
    
    template <typename T>
    explicit Response(int _id, std::optional<T> _data) : id(_id) {
        if (_data) {
            data = std::move(*_data);
        }
    }
    template <typename T>
    explicit Response(int _id, T&& _data) : id(_id), data(std::forward<T>(_data)) {}

    int Id() const { return id; }
    const auto& Data() const { return data; }
    
private:
    int id = 0;
    std::variant<NotFound, Bus, Stop, Route, Map> data;
};

class Database {
public:
    Database(const Json::Document& doc);

    Response Process(const Request& req) const {
        const auto id = req.Id();
        return std::visit([this, id](const auto& req) { return Response(id, Process(req)); }, req.Data());
    }

private:
    std::optional<Response::Bus> Process(const Request::Bus& req) const { return map.GetRouteInfo(req.name); }
    std::optional<Response::Stop> Process(const Request::Stop& req) const { return map.GetBusesFor(req.name); }
    std::optional<Response::Route> Process(const Request::Route& req) const {
        auto route = map.GetRoute(req.from, req.to);
        if (route) {
            auto img = map.Render(*route);
            return Response::Route{
                std::move(*route),
                std::move(img)
            };
        }
        return std::nullopt;
    }
    Response::Map Process(const Request::Map& req) const { return map.Render(); }

    RouteMap map;
};

Json::Document ProcessRequests(const Database& db, const Json::Document& doc);
