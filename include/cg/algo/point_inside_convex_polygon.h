#pragma once

#include <cg/primitives/contour.h>
#include <cg/primitives/my_rectangle.h>
#include <cg/primitives/segment.h>
#include <cg/operations/orientation.h>
#include <cg/algo/point_inside_triangle.h>
#include <cg/algo/segment_intersect.h>

namespace cg
{
   template<typename T>
   inline bool point_inside_convex_polygon(contour_2t<T> const & a, point_2t<T> const & b)
   {
      auto it = std::upper_bound(a.begin(), a.end(), b, [&a](const point_2t<T> &lhs, const point_2t<T> &rhs) {
                         return orientation(a[0], lhs, rhs) > 0;
      });
      if (it == a.end() && orientation(a[0], *(it-1), b) <= 0) 
         it--;
      return it != a.begin() && it != a.end() && orientation(*(it-1), *it, b) >= 0;
   }
}
