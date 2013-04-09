#pragma once

#include <cg/primitives/rectangle.h>
#include <cg/algo/segment_intersect.h>

namespace cg
{
   template<typename T>
   inline bool rectangle_segment_intersect(rectangle_2t<T> const & a, segment_2t<T> const & b)
   {
      if (a.contains(b[0]) || a.contains(b[1]))
         return true;
      segment_2t<T> diag1(a.corner(0, 0), a.corner(1, 1));
      segment_2t<T> diag2(a.corner(1, 0), a.corner(0, 1));
      return segment_intersect(diag1, b) || segment_intersect(diag2, b);
   }
}
