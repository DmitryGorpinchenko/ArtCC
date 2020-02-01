#pragma once

#include "route_map.h"
#include "json.h"

#include <optional>
#include <variant>

struct Endpoints {
    std::string from, to;
};
struct Request {
    enum class Type : int {
        BUS,
        STOP,
        ROUTE,
    };
    Type type;
    std::variant<std::monostate, std::string, Endpoints> data;
    int id;
    
    template <typename T>
    const auto& Data() const { return std::get<T>(data); }
};

class Response {
public:
    Response() = default;
    
    template <typename Info>
    explicit Response(int _id, std::optional<Info> _info) : id(_id) {
        if (_info) {
            data = *_info;
        }
    }

    int GetId() const { return id; }

    template <typename Info>
    bool Has() const { return std::holds_alternative<Info>(data); }
    template <typename Info>
    const auto& Get() const { return std::get<Info>(data); }
private:
    int id = 0;
    std::variant<std::monostate, RouteInfo, Buses, Route> data;
};

class Database {
public:
    Database(const Json::Document& doc);

    Response Process(const Request& req) const {
        switch (req.type) {
        case Request::Type::BUS:   return Response(req.id, map.GetRouteInfo(req.Data<std::string>()));
        case Request::Type::STOP:  return Response(req.id, map.GetBusesFor(req.Data<std::string>()));
        case Request::Type::ROUTE: return Response(req.id, map.GetRoute(req.Data<Endpoints>().from,
                                                                        req.Data<Endpoints>().to));
        }
        return {};
    }

private:
    RouteMap map;
};

Json::Document ProcessRequests(const Database& db, const Json::Document& doc);
