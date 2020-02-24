#pragma once

#include <iostream>
#include <variant>
#include <optional>
#include <string>
#include <vector>
#include <iterator>
#include <cstdint>

namespace Svg {

struct Point {
    double x = 0;
    double y = 0;
};

struct Rgb {
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    std::optional<double> alpha;
};

using None = std::monostate;
using Color = std::variant<None, std::string, Rgb>;

template <typename Derived>
class Obj {
public:
    Derived& SetFillColor(const Color& c) {
        fill_color = c;
        return AsDerived();
    }
    Derived& SetStrokeColor(const Color& c) {
        stroke_color = c;
        return AsDerived();
    }
    Derived& SetStrokeWidth(double w) {
        stroke_width = w;
        return AsDerived();
    }
    Derived& SetStrokeLineCap(const std::string& s) {
        cap = s;
        return AsDerived();
    }
    Derived& SetStrokeLineJoin(const std::string& s) {
        join = s;
        return AsDerived();
    }
    
    const Obj& AsBase() const { return *this; };
    
    template <typename V>
    friend std::ostream& operator <<(std::ostream& os, const Obj<V>& val);
    
private:
    Derived& AsDerived() { return static_cast<Derived&>(*this); }

    Color fill_color;
    Color stroke_color;
    std::optional<std::string> cap;
    std::optional<std::string> join;
    double stroke_width = 1.;
};

class Rect : public Obj<Rect> {
public:
    Rect& SetTopLeft(Point p) {
        top_left = p;
        return *this;
    }
    Rect& SetWidth(double _w) {
        w = _w;
        return *this;
    }
    Rect& SetHeight(double _h) {
        h = _h;
        return *this;
    }
    
    friend std::ostream& operator <<(std::ostream& os, const Rect& val);
    
private:
    Point top_left;
    double w = 0.;
    double h = 0.;
};

class Circle : public Obj<Circle> {
public:
    Circle& SetCenter(Point p) {
        center = p;
        return *this;
    }
    Circle& SetRadius(double r) {
        radius = r;
        return *this;
    }
    
    friend std::ostream& operator <<(std::ostream& os, const Circle& val);
    
private:
    Point center;
    double radius = 1.;
};

class Polyline : public Obj<Polyline> {
public:
    Polyline& AddPoint(Point p) {
        vertices.push_back(p);
        return *this;
    }
    Polyline& AddPoints(std::vector<Point> points) {
        vertices.insert(end(vertices),
                        make_move_iterator(begin(points)),
                        make_move_iterator(end(points)));
        return *this;
    }
    
    friend std::ostream& operator <<(std::ostream& os, const Polyline& val);
    
private:
    std::vector<Point> vertices;
};

class Text : public Obj<Text> {
public:
    Text& SetPoint(Point p) {
        coords = p;
        return *this;
    }
    Text& SetOffset(Point p) {
        offset = p;
        return *this;
    }
    Text& SetFontSize(uint32_t sz) {
        font_size = sz;
        return *this;
    }
    Text& SetFontWeight(const std::string& s) {
        font_weight = s;
        return *this;
    }
    Text& SetFontFamily(const std::string& s) {
        font = s;
        return *this;
    }
    Text& SetData(const std::string& s) {
        data = s;
        return *this;
    }
    
    friend std::ostream& operator <<(std::ostream& os, const Text& val);

private:
    Point coords;
    Point offset;
    std::string data;
    std::optional<std::string> font_weight;
    std::optional<std::string> font;
    uint32_t font_size = 1;
};

using Object = std::variant<Rect, Circle, Polyline, Text>;

class Document {
public:
    template <typename T>
    void Add(T&& obj) {
        objs.emplace_back(std::forward<T>(obj));
    }

    void Render(std::ostream& os) const;
private:
    std::vector<Object> objs;
};

}
