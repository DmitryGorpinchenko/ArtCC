#include "Common.h"

#include <algorithm>

using namespace std;

class Shape : public IShape {
public:
    void SetPosition(Point p) override { pos = p; }
    Point GetPosition() const override { return pos; }

    void SetSize(Size s) override { size = s; }
    Size GetSize() const override { return size; }

    void SetTexture(std::shared_ptr<ITexture> t) override { texture = move(t); }
    ITexture* GetTexture() const override { return texture.get(); }

    void Draw(Image& img) const override { DrawImpl(img); }

protected:
    Point ToLocal(Point p) const {
        return Point{p.x - pos.x, p.y - pos.y};
    }

private:
    void DrawImpl(Image& img) const {
        if (img.empty()) {
            return;
        }
        
        const int x_start = max(0, pos.x);
        const int y_start = max(0, pos.y);
        const int x_end = min(pos.x + size.width, int(img.front().size()));
        const int y_end = min(pos.y + size.height, int(img.size()));
        
        for (int i = y_start; i < y_end; ++i) {
            for (int j = x_start; j < x_end; ++j) {
                const Point p = {j, i};
                if (InShape(p)) {
                    img[p.y][p.x] = TextureAt(p);
                }
            }
        }
    }
    
    char TextureAt(Point p) const {
        p = ToLocal(p);
        if (texture) {
            const auto& img = texture->GetImage();
            if ((p.y < img.size()) && (p.x < img.front().size())) {
                return img[p.y][p.x];
            }
        }
        return '.';
    }
    
    virtual bool InShape(Point p) const {
        p = ToLocal(p);
        return (p.x < size_t(size.width)) && (p.y < size_t(size.height));
    }

    Point pos = { 0, 0 };
    Size size = { 0, 0 };
    std::shared_ptr<ITexture> texture;
};

class Ellipse : public Shape {
public:
    std::unique_ptr<IShape> Clone() const override { return make_unique<Ellipse>(*this); }
private:
    bool InShape(Point p) const override { return IsPointInEllipse(ToLocal(p), GetSize()); }
};

class Rectangle : public Shape {
public:
    std::unique_ptr<IShape> Clone() const override { return make_unique<Rectangle>(*this); }
};

unique_ptr<IShape> MakeShape(ShapeType shape_type) {
    switch (shape_type) {
    case ShapeType::Rectangle: return make_unique<Rectangle>();
    case ShapeType::Ellipse:   return make_unique<Ellipse>();
    default: break;
    }
    return {};
}
