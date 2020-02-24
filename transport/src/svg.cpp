#include "svg.h"

namespace Svg {

namespace {

std::ostream& operator <<(std::ostream& os, Point val) {
    os << val.x << ',' << val.y;
    return os;
}

std::ostream& operator <<(std::ostream& os, Rgb val) {
    os << "rgb";
    if (val.alpha) {
        os << 'a';
    }
    os << '('
       << int(val.red) << ','
       << int(val.green) << ','
       << int(val.blue);
    if (val.alpha) {
        os << ',' << *val.alpha;
    }
    os << ')';
    return os;
}

std::ostream& operator <<(std::ostream& os, None val) {
    return os << "none";
}

std::ostream& operator <<(std::ostream& os, const Color& val) {
    std::visit([&os](const auto& d) { os << d; }, val);
    return os;
}

} // end namespace

template <typename T>
std::ostream& operator <<(std::ostream& os, const Obj<T>& val) {
    os << "fill=" << '"' << val.fill_color << '"'
       << " stroke=" << '"' << val.stroke_color << '"'
       << " stroke-width=" << '"' << val.stroke_width << '"';
    if (val.cap) {
        os << " stroke-linecap=" << '"' << *val.cap << '"';
    }
    if (val.join) {
        os << " stroke-linejoin=" << '"' << *val.join << '"';
    }
    return os;
}

std::ostream& operator <<(std::ostream& os, const Rect& val) {
    os << "<rect"
       << " x=" << '"' << val.top_left.x << '"'
       << " y=" << '"' << val.top_left.y << '"'
       << " width=" << '"' << val.w << '"'
       << " height=" << '"' << val.h << '"'
       << ' ' << val.AsBase()
       << "/>";
    return os;
}

std::ostream& operator <<(std::ostream& os, const Circle& val) {
    os << "<circle"
       << " cx=" << '"' << val.center.x << '"'
       << " cy=" << '"' << val.center.y << '"'
       << " r=" << '"' << val.radius << '"'
       << ' ' << val.AsBase()
       << "/>";
    return os;
}

std::ostream& operator <<(std::ostream& os, const Polyline& val) {
    os << "<polyline"
       << " points="
       << '"';
    for (auto p : val.vertices) {
        os << p << ' ';
    }
    os << '"'
       << ' ' << val.AsBase()
       << "/>";
    return os;
}

std::ostream& operator <<(std::ostream& os, const Text& val) {
    os << "<text";
    os << " x=" << '"' << val.coords.x << '"'
       << " y=" << '"' << val.coords.y << '"'
       << " dx=" << '"' << val.offset.x << '"'
       << " dy=" << '"' << val.offset.y << '"'
       << " font-size=" << '"' << val.font_size << '"';
    if (val.font_weight) {
        os << " font-weight=" << '"' << *val.font_weight << '"';
    }
    if (val.font) {
        os << " font-family=" << '"' << *val.font << '"';
    }
    os << ' ' << val.AsBase();
    os << '>';
    os << val.data;
    os << "</text>";
    return os;
}

void Document::Render(std::ostream& os) const {
    os << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
       << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">";
    for (const auto& o : objs) {
        std::visit([&os](const auto& data) { os << data; }, o);
    }
    os << "</svg>";
}

}
