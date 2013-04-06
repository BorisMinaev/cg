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
      T type_max = std::numeric_limits<T>::max();
      segment_2t<T> cur_seg(b, point_2t<T>(type_max, b.y));
      for (size_t i = 0; i != a.vertices_num(); i++) {
         segment_2t<T> seg(a[i], a[(i + 1) % a.vertices_num()]);
         if (orientation(seg[0], seg[1], b) == 0 && min(seg) <= b && b <= max(seg))
            return true;
         if (seg[0].y == seg[1].y)
            continue;
         if (b.y > std::min(seg[0].y, seg[1].y) && b.y <= std::max(seg[0].y, seg[1].y) && segment_intersect(cur_seg, seg))
            num_intersections++;
      }
      return num_intersections % 2;
   }
}
