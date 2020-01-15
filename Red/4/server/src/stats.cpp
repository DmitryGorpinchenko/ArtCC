#include "stats.h"

Stats::Stats()
    : method_stats {
        { "GET",     0 },
        { "PUT",     0 },
        { "POST",    0 },
        { "DELETE",  0 },
        { "UNKNOWN", 0 },
    }
    , uri_stats {
        { "/",        0 },
        { "/order",   0 },
        { "/product", 0 },
        { "/basket",  0 },
        { "/help",    0 },
        { "unknown",  0 },
    }
{}

void Stats::AddMethod(string_view method) {
    if (auto it = method_stats.find(method); it != end(method_stats)) {
        ++(*it).second;
    } else {
        ++method_stats["UNKNOWN"];
    }
}

void Stats::AddUri(string_view uri) {
    if (auto it = uri_stats.find(uri); it != end(uri_stats)) {
        ++(*it).second;
    } else {
        ++uri_stats["unknown"];
    }
}

//

string_view ReadToken(string_view& line) {
    const size_t pos = line.find_first_not_of(' ');
    line.remove_prefix((pos != line.npos) ? pos : line.size());
    const auto res = line.substr(0, line.find(' '));
    line.remove_prefix(res.size());
    return res;
}

HttpRequest ParseRequest(string_view line) {
    const auto method = ReadToken(line);
    const auto uri = ReadToken(line);
    const auto protocol = ReadToken(line);
    return { method, uri, protocol };
}
