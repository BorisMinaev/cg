#include <gtest/gtest.h>

#include <cg/algo/rectangle_segment_intersect.h>
#include <cg/primitives/range.h>

TEST(test1, tests)
{
   cg::rectangle_2 r(cg::range(0, 10), cg::range(0, 10));
   cg::point_2 p21(3, 3);
   cg::point_2 p22(3, 100);
   cg::segment_2d ss(p21, p22);
   ASSERT_EQ(rectangle_segment_intersect(r, ss), true);
}

TEST(test2, tests)
{
   cg::rectangle_2 r(cg::range(0, 10), cg::range(0, 10));
   cg::point_2 p21(-10, -10);
   cg::point_2 p22(3, 100);
   cg::segment_2d ss(p21, p22);
   ASSERT_EQ(rectangle_segment_intersect(r, ss), false);
}


TEST(test3, tests)
{
   cg::rectangle_2 r(cg::range(0, 10), cg::range(0, 10));
   cg::point_2 p21(-10, -10);
   cg::point_2 p22(100, 100);
   cg::segment_2d ss(p21, p22);
   ASSERT_EQ(rectangle_segment_intersect(r, ss), true);
}

TEST(test4, tests)
{
   cg::rectangle_2 r(cg::range(20, 50), cg::range(0, 20));
   cg::point_2 p21(30, 15);
   cg::point_2 p22(10, 20);
   cg::segment_2d ss(p21, p22);
   ASSERT_EQ(rectangle_segment_intersect(r, ss), true);
}

TEST(test5, tests)
{
   cg::rectangle_2 r(cg::range(20, 50), cg::range(0, 20));
   cg::point_2 p21(50, -1);
   cg::point_2 p22(51, 1);
   cg::segment_2d ss(p21, p22);
   ASSERT_EQ(rectangle_segment_intersect(r, ss), false);
}
