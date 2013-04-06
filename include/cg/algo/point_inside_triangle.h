#pragma once

#include <cg/primitives/triangle.h>
#include <cg/operations/orientation.h>

namespace cg
{
   inline bool point_inside_triangle(triangle_2d const & a, point_2 const & b)
   {
      return (orientation(a[0], a[1], b) >= 0 && orientation(a[1], a[2], b) >= 0 && orientation(a[2], a[0], b) >= 0);
   }
}
