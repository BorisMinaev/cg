#pragma once

#include <cg/primitives/segment.h>
#include <cg/operations/orientation.h>
#include <cmath>

namespace cg
{
   template <typename T>
   inline bool segment_intersect(segment_2t<T> const & a, segment_2t<T> const & b)
   {
      int o1 = orientation(a[0], a[1], b[0]);
      int o2 = orientation(a[0], a[1], b[1]);
      if (o1 == o2 && o1 == 0) {
         return !(max(a) < min(b) || min(a) > max(b));
      } else {
         if (o1 == o2) {
            return false;
         }
         return orientation(b[0], b[1], a[0]) * orientation(b[0], b[1], a[1]) <= 0;
      }
   }
}
