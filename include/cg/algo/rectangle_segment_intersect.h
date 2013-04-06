#pragma once

#include <cg/primitives/triangle.h>
#include <cg/primitives/my_rectangle.h>
#include <cg/primitives/segment.h>
#include <cg/algo/point_inside_triangle.h>
#include <cg/algo/segment_intersect.h>

namespace cg
{

   inline bool rectangle_segment_intersect(my_rectangle_2d const & a, segment_2d const & b)
   {

      triangle_2d tr1(a[0], a[1], a[2]);
      triangle_2d tr2(a[0], a[2], a[3]);
      if (point_inside_triangle(tr1, b[0]) || point_inside_triangle(tr1, b[1]))
         return true;
      if (point_inside_triangle(tr2, b[0]) || point_inside_triangle(tr2, b[1]))
         return true;
      segment_2d s1(a[0], a[1]);
      segment_2d s2(a[1], a[2]);
      segment_2d s3(a[2], a[3]);
      segment_2d s4(a[3], a[0]);
      if (segment_intersect(s1, b) || segment_intersect(s2, b) || segment_intersect(s3, b) || segment_intersect(s4, b))
         return true;
      return false;
   }
}
