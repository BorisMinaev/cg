#include <gtest/gtest.h>

#include <cg/algo/triangle_segment_intersect.h>

TEST(test1, tests)
{
   cg::point_2 p11(0, 0);
   cg::point_2 p12(10, 0);
   cg::point_2 p13(10, 10);
   cg::point_2 p21(3, 3);
   cg::point_2 p22(3, 100);
   cg::triangle_2d tr(p11, p12, p13);
   cg::segment_2d ss(p21, p22);
   ASSERT_EQ(triangle_segment_intersect(tr, ss), true);
}

TEST(test2, tests)
{
   cg::point_2 p11(0, 0);
   cg::point_2 p12(10, 0);
   cg::point_2 p13(10, 10);
   cg::point_2 p21(-10, 0);
   cg::point_2 p22(20, 0);
   cg::triangle_2d tr(p11, p12, p13);
   cg::segment_2d ss(p21, p22);
   ASSERT_EQ(triangle_segment_intersect(tr, ss), true);
}

TEST(test3, tests)
{
   cg::point_2 p11(0, 0);
   cg::point_2 p12(10, 0);
   cg::point_2 p13(10, 10);
   cg::point_2 p21(-10, -10);
   cg::point_2 p22(10, -20);
   cg::triangle_2d tr(p11, p12, p13);
   cg::segment_2d ss(p21, p22);
   ASSERT_EQ(triangle_segment_intersect(tr, ss), false);
}

TEST(test4, tests)
{
   cg::point_2 p11(20, 10);
   cg::point_2 p12(10, 20);
   cg::point_2 p13(20, 00);
   cg::point_2 p21(-10, 20);
   cg::point_2 p22(100, 20);
   cg::triangle_2d tr(p11, p12, p13);
   cg::segment_2d ss(p21, p22);
   ASSERT_EQ(triangle_segment_intersect(tr, ss), true);
}

TEST(test5, tests)
{
   cg::point_2 p11(20, 10);
   cg::point_2 p12(10, 20);
   cg::point_2 p13(20, 00);
   cg::point_2 p21(10, -20);
   cg::point_2 p22(10, 0);
   cg::triangle_2d tr(p11, p12, p13);
   cg::segment_2d ss(p21, p22);
   ASSERT_EQ(triangle_segment_intersect(tr, ss), false);
}

TEST(test6, tests)
{
   cg::point_2 p11(20, 10);
   cg::point_2 p12(10, 20);
   cg::point_2 p13(20, 00);
   cg::point_2 p21(0, 0);
   cg::point_2 p22(10, 30);
   cg::triangle_2d tr(p11, p12, p13);
   cg::segment_2d ss(p21, p22);
   ASSERT_EQ(triangle_segment_intersect(tr, ss), false);
}

TEST(test7, tests)
{
   cg::point_2 p11(20, 10);
   cg::point_2 p12(10, 20);
   cg::point_2 p13(20, 00);
   cg::point_2 p21(0, 100);
   cg::point_2 p22(40, 20);
   cg::triangle_2d tr(p11, p12, p13);
   cg::segment_2d ss(p21, p22);
   ASSERT_EQ(triangle_segment_intersect(tr, ss), false);
}