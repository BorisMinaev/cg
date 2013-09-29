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

   template<class BidIter>
   bool is_farther(BidIter first, BidIter second, BidIter now, BidIter want) {
       double d1 = abs(vect_mul(*first, *second, *now));
       double d2 = abs(vect_mul(*first, *second, *want));
       return d2 > d1;
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
            if (!is_farther(first, second, farthest_point, next))
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
