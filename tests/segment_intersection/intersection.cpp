#include <gtest/gtest.h>

#include <cg/algo/segment_intersect.h>

TEST(test1, tests)
{
   cg::point_2 p11(0, 0);
   cg::point_2 p12(0, 0);
   cg::point_2 p21(1, 1);
   cg::point_2 p22(1, 1);
   cg::segment_2d s1(p11, p12);
   cg::segment_2d s2(p21, p22);
   ASSERT_EQ(segment_intersect(s1, s2), false);
}

TEST(test2, tests)
{
   cg::point_2 p11(1, 1);
   cg::point_2 p12(1, 1);
   cg::point_2 p21(1, 1);
   cg::point_2 p22(1, 1);
   cg::segment_2d s1(p11, p12);
   cg::segment_2d s2(p21, p22);
   ASSERT_EQ(segment_intersect(s1, s2), true);
}

TEST(test3, tests)
{
   cg::point_2 p11(0, 0);
   cg::point_2 p12(3, 3);
   cg::point_2 p21(1, 1);
   cg::point_2 p22(1, 1);
   cg::segment_2d s1(p11, p12);
   cg::segment_2d s2(p21, p22);
   ASSERT_EQ(segment_intersect(s1, s2), true);
}

TEST(test4, tests)
{
   cg::point_2 p11(0, 0);
   cg::point_2 p12(3, 3);
   cg::point_2 p21(3, 3);
   cg::point_2 p22(3, 3);
   cg::segment_2d s1(p11, p12);
   cg::segment_2d s2(p21, p22);
   ASSERT_EQ(segment_intersect(s1, s2), true);
}

TEST(test5, tests)
{
   cg::point_2 p11(3, 5);
   cg::point_2 p12(0, 0);
   cg::point_2 p21(0, 0);
   cg::point_2 p22(0, 0);
   cg::segment_2d s1(p11, p12);
   cg::segment_2d s2(p21, p22);
   ASSERT_EQ(segment_intersect(s1, s2), true);
}

TEST(test6, tests)
{
   cg::point_2 p11(1, 1);
   cg::point_2 p12(5, 1);
   cg::point_2 p21(3, 1);
   cg::point_2 p22(3, 4);
   cg::segment_2d s1(p11, p12);
   cg::segment_2d s2(p21, p22);
   ASSERT_EQ(segment_intersect(s1, s2), true);
}

TEST(test7, tests)
{
   cg::point_2 p11(1, 1);
   cg::point_2 p12(5, 1);
   cg::point_2 p21(3, 2);
   cg::point_2 p22(3, 4);
   cg::segment_2d s1(p11, p12);
   cg::segment_2d s2(p21, p22);
   ASSERT_EQ(segment_intersect(s1, s2), false);
}

TEST(test8, tests)
{
   cg::point_2 p11(1, 1);
   cg::point_2 p12(5, 1);
   cg::point_2 p21(3, -1);
   cg::point_2 p22(3, 0);
   cg::segment_2d s1(p11, p12);
   cg::segment_2d s2(p21, p22);
   ASSERT_EQ(segment_intersect(s1, s2), false);
}

TEST(test9, tests)
{
   cg::point_2 p11(0, 0);
   cg::point_2 p12(10, 10);
   cg::point_2 p21(10, 0);
   cg::point_2 p22(0, 10);
   cg::segment_2d s1(p11, p12);
   cg::segment_2d s2(p21, p22);
   ASSERT_EQ(segment_intersect(s1, s2), true);
}

TEST(test10, tests)
{
   cg::point_2 p11(0, 0);
   cg::point_2 p12(10, 10);
   cg::point_2 p21(10, 0);
   cg::point_2 p22(5, 5);
   cg::segment_2d s1(p11, p12);
   cg::segment_2d s2(p21, p22);
   ASSERT_EQ(segment_intersect(s1, s2), true);
}

TEST(test11, tests)
{
   cg::point_2 p11(0, 0);
   cg::point_2 p12(10, 10);
   cg::point_2 p21(10, 0);
   cg::point_2 p22(7, 3);
   cg::segment_2d s1(p11, p12);
   cg::segment_2d s2(p21, p22);
   ASSERT_EQ(segment_intersect(s1, s2), false);
}

TEST(test12, tests)
{
   cg::point_2 p11(0, 0);
   cg::point_2 p12(10, 10);
   cg::point_2 p21(5, 5);
   cg::point_2 p22(15, 15);
   cg::segment_2d s1(p11, p12);
   cg::segment_2d s2(p21, p22);
   ASSERT_EQ(segment_intersect(s1, s2), true);
}

TEST(test13, tests)
{
   cg::point_2 p11(0, 0);
   cg::point_2 p12(10, 10);
   cg::point_2 p21(11, 11);
   cg::point_2 p22(12, 12);
   cg::segment_2d s1(p11, p12);
   cg::segment_2d s2(p21, p22);
   ASSERT_EQ(segment_intersect(s1, s2), false);
}

TEST(test14, tests)
{
   cg::point_2 p11(0, 0);
   cg::point_2 p12(5, 5);
   cg::point_2 p21(5, 5);
   cg::point_2 p22(10, 0);
   cg::segment_2d s1(p11, p12);
   cg::segment_2d s2(p21, p22);
   ASSERT_EQ(segment_intersect(s1, s2), true);
}