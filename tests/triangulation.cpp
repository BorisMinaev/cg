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


TEST(triangulation, triangulation_parabola)
{
   using cg::point_2;


   std::vector<point_2> pts;
   int n = 1000;
   for (int i = n; i >= 0; i--) {
       double x = i/(n+0.);
       pts.push_back(point_2(1000*x, 1000*x*x));
   }

   cg::make_triangulation(pts.begin(), pts.end());
}
