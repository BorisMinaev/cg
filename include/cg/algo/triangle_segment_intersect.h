#pragma once

#include <cg/primitives/triangle.h>
#include <cg/primitives/segment.h>
#include <cg/algo/point_inside_triangle.h>
#include <cg/algo/segment_intersect.h>

namespace cg
{
   template <typename T>
   inline bool triangle_segment_intersect(triangle_2t<T> const & a, segment_2t<T> const & b)
   {
      if (point_inside_triangle(a, b[0]) || point_inside_triangle(a, b[1]))
         return true;
      return (segment_intersect(segment_2d(a[0], a[1]), b) || segment_intersect(segment_2d(a[1], a[2]), b) || segment_intersect(segment_2d(a[2], a[0]), b));
   }
}
