#pragma once

#include <cg/primitives/contour.h>
#include <cg/primitives/segment.h>
#include <cg/operations/orientation.h>
#include <cg/algo/segment_intersect.h>

namespace cg
{
   template<typename T>
   inline bool point_inside_polygon(contour_2t<T> const & a, point_2t<T> const & b)
   {
      size_t num_intersections = 0;
      for (size_t i = 0; i != a.vertices_num(); i++) {
         point_2t<T> min_point = a[i];
         point_2t<T> max_point = a[(i + 1) % a.vertices_num()];
         if (min_point.y > max_point.y)
            std::swap(min_point, max_point);
         if (max_point.y <= b.y || min_point.y > b.y)
            continue;
         int orient = orientation(min_point, max_point, b);
         if (orient == 0)
            return true;
         if (orient > 0)
            num_intersections++;
      }
      return num_intersections % 2;
   }
}
