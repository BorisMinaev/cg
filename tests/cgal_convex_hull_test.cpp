#include <gtest/gtest.h>

#include <boost/assign/list_of.hpp>

#include <cg/convex_hull/graham.h>
#include <cg/convex_hull/andrew.h>
#include <cg/convex_hull/jarvis.h>
#include <cg/operations/contains/segment_point.h>
#include <cg/convex_hull/cgal_usage.h>

#include "random_utils.h"

template <class FwdIter>
bool is_convex_hull(FwdIter p, FwdIter c, FwdIter q)
{
   for (FwdIter t = boost::prior(c), s = p; s != c; t = s++)
   {
      for (FwdIter b = p; b != q; ++b)
      {
         switch (orientation(*t, *s, *b))
         {
         case cg::CG_RIGHT: return false;
         case cg::CG_COLLINEAR: if(!collinear_are_ordered_along_line(*t, *b, *s)) return false;
         case cg::CG_LEFT: continue;
         }
      }
   }

   return true;
}

TEST(cgal_usage_simple_Test, simple)
{
   using cg::point_2;

   std::vector<point_2> pts = boost::assign::list_of(point_2(0, 0))
                                                    (point_2(1, 0))
                                                    (point_2(0, 1))
                                                    (point_2(2, 0))
                                                    (point_2(0, 2))
                                                    (point_2(3, 0));

   EXPECT_TRUE(is_convex_hull(pts.begin(), cg::cgal_andrew_hull(pts.begin(), pts.end()), pts.end()));
}

TEST(cgal_usage_simple_Test, simple2)
{
   using cg::point_2;

   std::vector<point_2> pts = boost::assign::list_of(point_2(0, 0))
                                                    (point_2(1, 1))
                                                    (point_2(2, 2))
                                                    (point_2(3, 3))
                                                    (point_2(4, 4))
                                                    (point_2(5, 5));

   EXPECT_TRUE(is_convex_hull(pts.begin(), cg::cgal_andrew_hull(pts.begin(), pts.end()), pts.end()));
}

TEST(cgal_usage_simple_Test, uniform)
{
   using cg::point_2;

   std::vector<point_2> pts = uniform_points(1000000);
   EXPECT_TRUE(is_convex_hull(pts.begin(), cg::cgal_andrew_hull(pts.begin(), pts.end()), pts.end()));
}
