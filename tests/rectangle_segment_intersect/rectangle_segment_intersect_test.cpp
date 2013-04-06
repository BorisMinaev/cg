#include <gtest/gtest.h>

#include <cg/algo/rectangle_segment_intersect.h>

TEST(test1, tests)
{
   cg::point_2 p11(0, 0);
   cg::point_2 p12(10, 0);
   cg::point_2 p13(10, 10);
   cg::point_2 p14(0, 10);
   cg::point_2 p21(3, 3);
   cg::point_2 p22(3, 100);
   cg::my_rectangle_2d tr(p11, p12, p13, p14);
   cg::segment_2d ss(p21, p22);
   ASSERT_EQ(rectangle_segment_intersect(tr, ss), true);
}

TEST(test2, tests)
{
   cg::point_2 p11(0, 0);
   cg::point_2 p12(10, 0);
   cg::point_2 p13(10, 10);
   cg::point_2 p14(0, 10);
   cg::point_2 p21(-10, -10);
   cg::point_2 p22(3, 100);
   cg::my_rectangle_2d tr(p11, p12, p13, p14);
   cg::segment_2d ss(p21, p22);
   ASSERT_EQ(rectangle_segment_intersect(tr, ss), false);
}

TEST(test3, tests)
{
   cg::point_2 p11(0, 0);
   cg::point_2 p12(10, 0);
   cg::point_2 p13(10, 10);
   cg::point_2 p14(0, 10);
   cg::point_2 p21(-10, -10);
   cg::point_2 p22(100, 100);
   cg::my_rectangle_2d tr(p11, p12, p13, p14);
   cg::segment_2d ss(p21, p22);
   ASSERT_EQ(rectangle_segment_intersect(tr, ss), true);
}

TEST(test4, tests)
{
   cg::point_2 p11(20, 20);
   cg::point_2 p12(20, 0);
   cg::point_2 p13(50, 0);
   cg::point_2 p14(50, 20);
   cg::point_2 p21(30, 15);
   cg::point_2 p22(10, 20);
   cg::my_rectangle_2d tr(p11, p12, p13, p14);
   cg::segment_2d ss(p21, p22);
   ASSERT_EQ(rectangle_segment_intersect(tr, ss), true);
}

TEST(test5, tests)
{
   cg::point_2 p11(20, 20);
   cg::point_2 p12(20, 0);
   cg::point_2 p13(50, 0);
   cg::point_2 p14(50, 20);
   cg::point_2 p21(50, -1);
   cg::point_2 p22(51, 1);
   cg::my_rectangle_2d tr(p11, p12, p13, p14);
   cg::segment_2d ss(p21, p22);
   ASSERT_EQ(rectangle_segment_intersect(tr, ss), false);
}
