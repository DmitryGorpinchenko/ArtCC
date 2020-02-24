#pragma once

#include <complex>

namespace Geo {

class Point {
public:
    Point(double _lat_deg = 0, double _lon_deg = 0);
    
    double LatDeg() const { return lat_deg; }
    double LonDeg() const { return lon_deg; }
    
    const std::complex<double>& Lat() const { return lat; }
    const std::complex<double>& Lon() const { return lon; }
private:
    double lat_deg, lon_deg;
    std::complex<double> lat, lon;
};

double GreatCircleDistance(const Point& p1, const Point& p2);

}
