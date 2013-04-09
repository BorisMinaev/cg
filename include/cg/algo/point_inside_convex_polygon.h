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
   /*
   inline bool point_inside_convex_polygon(contour_2t<T> const & a, point_2t<T> const & p)
   {
      if (orientation(a[0], a[1], p) == CG_RIGHT)
         return false;
      auto it = std::lower_bound(a.begin(), a.end(), p, [&a](const point_2t<T> &lhs, const point_2t<T> &rhs) {
                         return orientation(a[0], lhs, rhs) == CG_LEFT;
      });
      if (it == a.end()) 
         return false;
      return orientation(*(it-1), *it, p) != CG_RIGHT;
   }
*/
   inline bool point_inside_convex_polygon(contour_2t<T> const & cont, point_2t<T> const & p) {
       if (orientation(cont[0], cont[1], p) == CG_RIGHT) return false;
       auto it = std::lower_bound(cont.begin() + 2, cont.end(), p, [&cont](const point_2t<T> &lhs, const point_2t<T> &rhs) {
         return orientation(cont[0], lhs, rhs) == CG_LEFT;
       });
    if (it == cont.end()) {
        return false;
    }
    return orientation(*(it - 1), *it, p) != CG_RIGHT;
     }
}
