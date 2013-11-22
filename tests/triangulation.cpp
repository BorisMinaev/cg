#include <gtest/gtest.h>

#include <boost/assign/list_of.hpp>

#include <cg/primitives/triangle.h>
#include <cg/triangulation/triangulation-point.h>

#include "random_utils.h"

TEST(triangulation, triangulation_performance)
{
   using cg::point_2;

   std::vector<point_2> pts = uniform_points(10000, 787788);

   cg::make_triangulation(pts.begin(), pts.end());
}
