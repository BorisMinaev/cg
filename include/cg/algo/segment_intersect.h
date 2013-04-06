#pragma once

#include <cg/primitives/segment.h>
#include <cg/operations/orientation.h>
#include <cmath>

namespace cg
{

   inline bool intersect(double l1, double r1, double l2, double r2) {
      return !(r1 < l2 || r2 < l1);
   }

   inline bool segment_intersect(segment_2d const & a, segment_2d const & b)
   {
      if (!intersect(std::min(a[0].x, a[1].x), std::max(a[0].x, a[1].x), std::min(b[0].x, b[1].x), std::max(b[0].x, b[1].x)))
         return false;
      if (!intersect(std::min(a[0].y, a[1].y), std::max(a[0].y, a[1].y), std::min(b[0].y, b[1].y), std::max(b[0].y, b[1].y)))
         return false;
      return orientation(a[0], a[1], b[0]) * orientation(a[0], a[1], b[1]) <= 0 &&
             orientation(b[0], b[1], a[0]) * orientation(b[0], b[1], a[1]) <= 0;
   }
}
