#include <gtest/gtest.h>

#include <cg/algo/inside_triangle.h>

TEST(test1, tests)
{
   cg::point_2 p11(0, 0);
   cg::point_2 p12(10, 0);
   cg::point_2 p13(10, 10);
   cg::point_2 p2(3, 3);
   cg::triangle_2d tr(p11, p12, p13);
   ASSERT_EQ(inside_triangle(tr, p2), true);
}

TEST(test2, tests)
{
   cg::point_2 p11(0, 0);
   cg::point_2 p12(10, 0);
   cg::point_2 p13(10, 10);
   cg::point_2 p2(0, 0);
   cg::triangle_2d tr(p11, p12, p13);
   ASSERT_EQ(inside_triangle(tr, p2), true);
}

TEST(test3, tests)
{
   cg::point_2 p11(0, 0);
   cg::point_2 p12(10, 0);
   cg::point_2 p13(10, 10);
   cg::point_2 p2(10, 5);
   cg::triangle_2d tr(p11, p12, p13);
   ASSERT_EQ(inside_triangle(tr, p2), true);
}

TEST(test4, tests)
{
   cg::point_2 p11(0, 0);
   cg::point_2 p12(10, 0);
   cg::point_2 p13(10, 10);
   cg::point_2 p2(0, 5);
   cg::triangle_2d tr(p11, p12, p13);
   ASSERT_EQ(inside_triangle(tr, p2), false);
}

TEST(test5, tests)
{
   cg::point_2 p11(0, 0);
   cg::point_2 p12(10, 0);
   cg::point_2 p13(10, 10);
   cg::point_2 p2(-10, -10);
   cg::triangle_2d tr(p11, p12, p13);
   ASSERT_EQ(inside_triangle(tr, p2), false);
}

TEST(test6, tests)
{
   cg::point_2 p11(10, 10);
   cg::point_2 p12(0, 0);
   cg::point_2 p13(20, 5);
   cg::point_2 p2(5, 5);
   cg::triangle_2d tr(p11, p12, p13);
   ASSERT_EQ(inside_triangle(tr, p2), true);
}

TEST(test7, tests)
{
   cg::point_2 p11(10, 10);
   cg::point_2 p12(0, 0);
   cg::point_2 p13(20, 5);
   cg::point_2 p2(5, 6);
   cg::triangle_2d tr(p11, p12, p13);
   ASSERT_EQ(inside_triangle(tr, p2), false);
}

TEST(test8, tests)
{
   cg::point_2 p11(10, 10);
   cg::point_2 p12(0, 0);
   cg::point_2 p13(20, 5);
   cg::point_2 p2(6, 5);
   cg::triangle_2d tr(p11, p12, p13);
   ASSERT_EQ(inside_triangle(tr, p2), true);
}