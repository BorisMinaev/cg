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
      point_2t<T> max_point = b[0];
      point_2t<T> min_point = b[1];
      if (max_point.y < min_point.y)
      	std::swap(min_point, max_point);
      if (min_point.x > max_point.x)
      	return segment_intersect(segment_2t<T>(a.corner(0, 0), a.corner(1, 1)), b);
      return segment_intersect(segment_2t<T>(a.corner(1, 0), a.corner(0, 1)), b);
   }
}
