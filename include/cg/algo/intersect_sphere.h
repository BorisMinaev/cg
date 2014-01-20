#pragma once

#include <cg/primitives/segment.h>
#include <cg/operations/orientation.h>
#include <cmath>

namespace cg
{

struct point_3 {
    double x, y, z;

    point_3(double x, double y, double z) : x(x) ,y(y), z(z) {}

    point_3(double longt, double lat) {
        z = sin(longt);
        double xy = cos(longt);
        x = xy * cos(lat);
        y = xy * sin(lat);
    }

    point_3() {
        x = y = z = 0;
    }

    //  [-pi/2..PI/2]
    double get_longtitude() const {
        return asin(z);
    }

    // [0..2PI)
    double get_latitude() const {
        double res = atan2(y, x);
        if (res < 0)
            res += asin(1.0) * 4;
        return res;
    }

    void negative() {
        x *= -1;
        y *= -1;
        z *= -1;
    }

    double tot() {
        return sqrt(x*x+y*y+z*z);
    }

    void normalize() {
        double sum = tot();
        x /= sum;
        y /= sum;
        z /= sum;
    }
};

point_3 const operator^ (point_3 const & a, point_3 const & b)
{
   return point_3(a.y * b.z - a.z * b.y,
                  a.z * b.x - a.x * b.z,
                  a.x * b.y - a.y * b.x);
}

const double eps = 1e-9;

int const operator* (point_3 const & a, point_3 const & b)
{
   double res = a.x * b.x + a.y * b.y + a.z * b.z;
   if (res> eps)
       return 1;
   if (res < -eps)
       return -1;
   return 0;
}

// [ab] x [cd]
boost::optional<point_3> intersection(const point_3 &a,const  point_3 &b, const point_3 &c, const point_3 &d) {
    point_3 normalAB = a ^ b;
    point_3 normalCD = c ^ d;
    normalAB.normalize();
    normalCD.normalize();
    int m1 = (normalAB * c), m2 = (normalAB * d);
    int m3 = (normalCD * a), m4 = (normalCD * b);
    if (m1 * m2 > 0)
        return boost::none;
    if (m3 * m4 > 0)
        return boost::none;
    point_3 result = normalAB ^ normalCD;
    if ((m1 == 0 && m2 == 0) || (m3 == 0 && m4 == 0)) {
        return boost::none;
    }
    result.normalize();
    point_3 midAB((a.x+b.x) / 2, (a.y+b.y)/2, (a.z+b.z) /2);
    midAB.normalize();
    point_3 midCD((c.x+d.x) / 2, (c.y+d.y)/2, (c.z+d.z) /2);
    midCD.normalize();
    if (midAB * result > 0 && midCD * result > 0) {
        return result;
    }
    result.negative();
    if (midAB * result > 0 && midCD * result > 0) {
        return result;
    }
    return boost::none;
}

}
