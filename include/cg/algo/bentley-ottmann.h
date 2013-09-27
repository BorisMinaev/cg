#pragma once

#include <cg/primitives/segment.h>
#include <cg/operations/orientation.h>
#include <cmath>

namespace cg
{

   std::vector<point_2f> segments_intersection(std::vector<segment_2f> & segments) {
      std::vector<point_2f> result;
      result.push_back(point_2f(5.0, 5.0));

      return result;
   }
}
