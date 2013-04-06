#include <gtest/gtest.h>

#include <cg/operations/orientation.h>
#include "random_utils.h"

TEST(orientation, uniform_line)
{
   boost::random::mt19937 gen;
   boost::random::uniform_real_distribution<> distr(-(1LL << 53), (1LL << 53));

   std::vector<cg::point_2> pts = uniform_points(1000);
   for (size_t l = 0, ln = 1; ln < pts.size(); l = ln++)
   {
      cg::point_2 a = pts[l];
      cg::point_2 b = pts[ln];

      for (size_t k = 0; k != 1000; ++k)
      {
         double t = distr(gen);
         cg::point_2 c = a + t * (b - a);

         ASSERT_EQ(*cg::orientation_r()(a, b, c), cg::orientation(a, b, c)) 
         << a.x << " " << a.y << std::endl 
         << b.x << " " << b.y << std::endl
         << c.x << " " << c.y << std::endl;
      }
   }
}