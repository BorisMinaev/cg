#pragma once

#include <cg/primitives/segment.h>
#include <cg/operations/orientation.h>
#include <boost/numeric/interval.hpp>
#include <cmath>
#include <gmpxx.h>

namespace cg
{
   typedef boost::numeric::interval_lib::unprotect<boost::numeric::interval<float> >::type interval;

   struct new_line {
      segment_2f s;
      interval A, B, C;

      new_line(segment_2f s) : s(s) {
         A = interval(s[1].y - s[0].y);
         B = interval(s[0].x - s[1].x);
         C = -(s[0].x * A + s[0].y * B);
      };
   };

   struct new_line_mpq {
      mpq_class A, B, C;

      new_line_mpq(new_line l) {
         A = mpq_class(l.s[1].y) - l.s[0].y;
         B = mpq_class(l.s[0].x) - l.s[1].x;
         C = -mpq_class(l.s[0].x) * A - mpq_class(l.s[0].y) * B;
      }
   };

   point_2t<interval> intersection(new_line l1, new_line l2) {
      interval x = (l2.C * l1.B - l1.C * l2.B) / (l1.A * l2.B - l2.A * l1.B);
      interval y = (l1.C * l2.A - l2.C * l1.A) / (l1.A * l2.B - l2.A * l1.B);
      return point_2t<interval>(x, y);
   }

   point_2t<mpq_class> intersection_mpq(new_line l_1, new_line l_2) {
      new_line_mpq l1(l_1);
      new_line_mpq l2(l_2);
      mpq_class x = (l2.C * l1.B - l1.C * l2.B) / (l1.A * l2.B - l2.A * l1.B);
      mpq_class y = (l1.C * l2.A - l2.C * l1.A) / (l1.A * l2.B - l2.A * l1.B);
      return point_2t<mpq_class>(x, y);
   }

   struct new_point {
      float x, y;
      bool is_precise;
      segment_2f s1, s2;
      point_2t<interval> interval_value;

      new_point(float x, float y) : x(x), y(y), is_precise(true) {};
      new_point(segment_2f s1, segment_2f s2) : is_precise(false), s1(s1), s2(s2) {
         interval_value = intersection(new_line(s1), new_line(s2));
      };
   };

   bool less_rational(new_point pp1, new_point pp2) {
      point_2t<mpq_class> p1 = intersection_mpq(new_line(pp1.s1), new_line(pp1.s2));
      point_2t<mpq_class> p2 = intersection_mpq(new_line(pp2.s1), new_line(pp2.s2));
      return (p1.x < p2.x) || (p1.x == p2.x && p1.y < p2.y);
   }

   bool less_exect_rational(new_point pp1, new_point pp2) {
      point_2t<mpq_class> p1(mpq_class(pp1.x), mpq_class(pp1.y));
      point_2t<mpq_class> p2 = intersection_mpq(new_line(pp2.s1), new_line(pp2.s2));
      return (p1.x < p2.x) || (p1.x == p2.x && p1.y < p2.y);
   }

   bool less_rational_exact(new_point pp1, new_point pp2) {
      point_2t<mpq_class> p1 = intersection_mpq(new_line(pp1.s1), new_line(pp1.s2));
      point_2t<mpq_class> p2(mpq_class(pp2.x), mpq_class(pp2.y));
      return (p1.x < p2.x) || (p1.x == p2.x && p1.y < p2.y);
   }

   bool mless(new_point p1, new_point p2) {
      if (p1.is_precise && p2.is_precise)
         return (p1.x < p2.x) || (p1.x == p2.x && p1.y < p2.y);
      if (!p1.is_precise && !p2.is_precise) {
         if (p1.interval_value.x.upper() < p2.interval_value.x.lower())
            return true;
         if (p1.interval_value.x.lower() > p2.interval_value.x.upper())
            return false;
         return less_rational(p1, p2);
      }
      if (p1.is_precise && !p2.is_precise) {
         if (p1.x < p2.interval_value.x.lower())
            return true;
         if (p1.x > p2.interval_value.x.upper())
            return false;
         return less_exect_rational(p1, p2);
      }
      if (p1.interval_value.x.upper() < p2.x)
         return true;
      if (p1.interval_value.x.lower() > p2.x)
            return false;
      return less_rational_exact(p1, p2);
   }

   std::vector<point_2f> segments_intersection(std::vector<segment_2f> & segments) {
      std::vector<point_2f> result;
      result.push_back(point_2f(5.0, 5.0));

      return result;
   }
}
