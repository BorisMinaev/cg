#pragma once

#include <cg/primitives/segment.h>
#include <cg/io/point.h>
#include <cg/operations/orientation.h>
#include <cg/convex_hull/graham.h>
#include <cmath>

namespace cg
{
template<class Scalar>
double dist2(point_2t<Scalar> const & a, point_2t<Scalar> const & b) {
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    return dx*dx+dy*dy;
}

template<class Scalar>
double vect_mul(point_2t<Scalar> const & a, point_2t<Scalar> const & b, point_2t<Scalar> const & c) {
    double dx1 = b.x - a.x;
    double dy1 = b.y - a.y;
    double dx2 = c.x - a.x;
    double dy2 = c.y - a.y;
    return dx1 * dy2 - dx2 * dy1;
}

typedef boost::numeric::interval_lib::unprotect<boost::numeric::interval<double> >::type interval;

template<class Scalar>
bool is_farther(point_2t<Scalar> a, point_2t<Scalar> b, point_2t<Scalar> c, point_2t<Scalar> d) {
    {
        double eps = std::numeric_limits<double>::epsilon() * 8;
        double dx1 = double(b.x) - a.x;
        double dy1 = double(b.y) - a.y;
        double dx2 = double(c.x) - a.x;
        double dy2 = double(c.y) - a.y;
        double dx3 = double(d.x) - a.x;
        double dy3 = double(d.y) - a.y;
        double vec_mul1 = abs(dx1 * dy2 - dx2 * dy1);
        double vec_mul2 = abs(dx1 * dy3 - dx3 * dy1);
        eps *= (vec_mul1 + vec_mul2);
        if (vec_mul2 > vec_mul1 + eps)
            return true;
        if (vec_mul2 + eps <= vec_mul1)
            return false;
    }
    {
        interval dx1 = interval(b.x) - a.x;
        interval dy1 = interval(b.y) - a.y;
        interval dx2 = interval(c.x) - a.x;
        interval dy2 = interval(c.y) - a.y;
        interval dx3 = interval(d.x) - a.x;
        interval dy3 = interval(d.y) - a.y;
        interval vec_mul1 = abs(dx1 * dy2 - dx2 * dy1);
        interval vec_mul2 = abs(dx1 * dy3 - dx3 * dy1);
        if (vec_mul2.lower() > vec_mul1.upper())
            return true;
        if (vec_mul2.upper() <= vec_mul1.lower())
            return false;
    }
    {
        mpq_class dx1 = mpq_class(b.x) - a.x;
        mpq_class dy1 = mpq_class(b.y) - a.y;
        mpq_class dx2 = mpq_class(c.x) - a.x;
        mpq_class dy2 = mpq_class(c.y) - a.y;
        mpq_class dx3 = mpq_class(d.x) - a.x;
        mpq_class dy3 = mpq_class(d.y) - a.y;
        mpq_class vec_mul1 = abs(dx1 * dy2 - dx2 * dy1);
        mpq_class vec_mul2 = abs(dx1 * dy3 - dx3 * dy1);
        return vec_mul2 > vec_mul1;
    }
}

template <class BidIter>
std::pair<BidIter, BidIter> farthest_points(BidIter begin, BidIter end) {
    if (begin == end) {
        return std::make_pair(begin, begin);
    }
    typedef typename BidIter::value_type mtype;
    std::vector<mtype> points(begin, end);
    points.erase(graham_hull(points.begin(), points.end()), points.end());
    std::pair<mtype, mtype> result;
    BidIter farthest_point = points.begin();
    double best_dist = -1;
    for (BidIter first = (points.end() - 1), second = points.begin(); second != points.end(); first = second++) {
        while (true) {
            BidIter next = farthest_point+1;
            if (next == points.end()) {
                next = points.begin();
            }
            if (!is_farther(*first, *second, *farthest_point, *next))
                break;
            farthest_point = next;
        }
        double d1 = dist2(*first, *farthest_point);
        if (d1 > best_dist) {
            best_dist = d1;
            result = std::make_pair(*first, *farthest_point);
        }
        double d2 = dist2(*second, *farthest_point);
        if (d2 > best_dist) {
            best_dist = d2;
            result = std::make_pair(*second, *farthest_point);
        }
    }
    return std::make_pair(std::find(begin, end, result.first), std::find(begin, end, result.second));
}
}
