#pragma once

#include <complex>

namespace Geo {

class Point {
public:
    Point(double _lat_deg = 0, double _lon_deg = 0);
    
    const std::complex<double>& Lat() const { return lat; }
    const std::complex<double>& Lon() const { return lon; }
private:
    std::complex<double> lat, lon;
};

double GreatCircleDistance(const Point& p1, const Point& p2);

}
