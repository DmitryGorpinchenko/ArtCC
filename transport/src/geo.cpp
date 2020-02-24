#include "geo.h"

#include <cmath>
using namespace std;

namespace {

const double PI = 3.1415926535;
const double EARTH_RADIUS_M = 6371000.;

double ToRad(double deg) {
    return deg * PI / 180;
}

complex<double> ComplexPhase(double deg) {
    return polar(1., ToRad(deg));
}

} // end namespace

namespace Geo {

Point::Point(double _lat_deg, double _lon_deg)
    : lat_deg(_lat_deg)
    , lon_deg(_lon_deg)
    , lat(ComplexPhase(_lat_deg))
    , lon(ComplexPhase(_lon_deg))
{}

double GreatCircleDistance(const Point& p1, const Point& p2) {
    const auto& p1_lat = p1.Lat();
    const auto& p2_lat = p2.Lat();
    const auto& p1_lon = p1.Lon();
    const auto& p2_lon = p2.Lon();
    const auto cos_delta_lon = p1_lon.real() * p2_lon.real() + p1_lon.imag() * p2_lon.imag();
    const auto theta = acos(p1_lat.imag() * p2_lat.imag() + p1_lat.real() * p2_lat.real() * cos_delta_lon);
    return EARTH_RADIUS_M * theta;
}

}
