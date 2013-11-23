#include <gtest/gtest.h>

#include <boost/assign/list_of.hpp>

#include <cg/primitives/triangle.h>
#include <cg/triangulation/triangulation-point.h>
#include <cg/operations/in_circle.h>

#include "random_utils.h"

TEST(triangulation, triangulation_performance)
{
   using cg::point_2;

   std::vector<point_2> pts = uniform_points(10000, 787788);

   cg::make_triangulation(pts.begin(), pts.end());
}

TEST(triangulation, triangulation_check)
{
   using cg::point_2;
   using cg::triangle_2;

    for (int test_id = 0; test_id < 100; test_id++) {
       std::vector<point_2> pts = uniform_points(30, test_id);

       std::vector<triangle_2> result = cg::make_triangulation(pts.begin(), pts.end());

       for (size_t i = 0; i <result.size(); i++) {
           for (size_t j = 0; j < pts.size(); j++) {
               if (in_circle(result[i][0], result[i][1], result[i][2], pts[j]))
                    ASSERT_TRUE(false);
           }
       }
    }
}


TEST(triangulation, triangulation_parabola)
{
   using cg::point_2;


   std::vector<point_2> pts;
   int n = 500;
   for (int i = n; i >= 0; i--) {
       double x = i/(n+0.);
       pts.push_back(point_2(1000*x, 1000*x*x));
   }

   cg::make_triangulation(pts.begin(), pts.end());
}
