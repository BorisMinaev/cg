#pragma once

#include <cg/primitives/segment.h>
#include <cg/io/point.h>
#include <cg/operations/orientation.h>
#include <cmath>

namespace cg
{
   template <class Scalar>
   double vect_mul(point_2t<Scalar> const & a, point_2t<Scalar> const & b) {
      return a.x * b.y - a.y * b.x;
   }

   template <class Scalar>
   double scal_mul(point_2t<Scalar> const & a, point_2t<Scalar> const & b) {
      return a.x * b.x + a.y * b.y;
   }

   double abs(double x) {
      return x < 0 ? -x : x;
   }

   template <class Scalar>
   double dist(point_2t<Scalar> a, point_2t<Scalar> b, point_2t<Scalar> c) {
      point_2t<Scalar> r12(b.x - a.x, b.y - a.y);
      point_2t<Scalar> r1(c.x - a.x, c.y - a.y);
      point_2t<Scalar> r2(c.x - b.x, c.y - b.y);
      double rr1 = sqrt(scal_mul(r1, r1));
      double rr2 = sqrt(scal_mul(r2, r2));
      double rr12 = sqrt(scal_mul(r12, r12));
      if (rr1 >= sqrt(rr2*rr2+rr12*rr12))
         return rr2;
      if (rr2 >= sqrt(rr1*rr1+rr12*rr12))
         return rr1;
      double v1 = vect_mul(r12, r1);
      if (v1 == 0)
         return 0;
      return abs(vect_mul(r12, r1) / sqrt(scal_mul(r12, r12)));
   }

   template <class BidIter, class OutIter>
   OutIter douglas_peucker(BidIter begin, BidIter end, OutIter out, double eps, bool needOutFirst = true) {
      point_2 point_begin = *begin;
      point_2 point_end = *(end - 1);
      if (needOutFirst) 
         *out++ = point_begin;
      if (begin + 1 == end && needOutFirst)
         return out;
      if (begin == end)
         return out;
      double maxDist = -1;
      BidIter maxIter = begin;
      for (BidIter it = begin; it != end; it++) {
         double d = dist(point_begin, point_end, *it);
         if (d > maxDist) {
            maxDist = d;
            maxIter = it;
         }
      }
      if (maxDist <= eps) {
         *out++ = point_end;
         return out;
      }
      douglas_peucker(begin, (maxIter+1), out, eps, false);
      douglas_peucker((maxIter+1), end, out, eps, false);
      return out;
   }
}
