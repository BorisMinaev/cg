#include <gtest/gtest.h>

#include <boost/assign/list_of.hpp>

#include <cg/primitives/point.h>
#include <cg/primitives/segment.h>
#include <cg/algo/bentley-ottmann.h>

#include "random_utils.h"

TEST(simple, test0)
{
    using cg::point_2;
    using cg::segment_2;

    segment_2 s1(point_2(0.0, 0.0), point_2(10.0, 10.0));
    segment_2 s2(point_2(0.0, 10.0), point_2(10.0, 0.0));

    std::vector<segment_2> a;
    a.push_back(s1);
    a.push_back(s2);

    EXPECT_EQ(cg::segments_intersection(a).size(), 1);
}

TEST(simple, test1)
{
    using cg::point_2;
    using cg::segment_2;

    segment_2 s1(point_2(0.0, 0.0), point_2(10.0, 10.0));
    segment_2 s2(point_2(0.0, 10.0), point_2(10.0, 0.0));
    segment_2 s3(point_2(0.0, 5.0), point_2(10.0, 5.0));

    std::vector<segment_2> a;
    a.push_back(s1);
    a.push_back(s2);
    a.push_back(s3);

    EXPECT_EQ(cg::segments_intersection(a).size(), 1);
}

TEST(simple, test2)
{
    using cg::point_2;
    using cg::segment_2;

    segment_2 s1(point_2(0.0, 0.0), point_2(10.0, 10.0));
    segment_2 s2(point_2(0.0, 10.0), point_2(10.0, 0.0));
    segment_2 s3(point_2(0.0, 5.0), point_2(10.0, 5.0));
    segment_2 s4(point_2(10.0, 5.0), point_2(10.0, 10.0));

    std::vector<segment_2> a;
    a.push_back(s1);
    a.push_back(s2);
    a.push_back(s3);
    a.push_back(s4);

    EXPECT_EQ(cg::segments_intersection(a).size(), 3);
}

TEST(simple, test3)
{
    using cg::point_2;
    using cg::segment_2;

    segment_2 s1(point_2(0.0, 0.0), point_2(10.0, 0.0));
    segment_2 s2(point_2(0.0, 10.0), point_2(10.0, 10.0));
    segment_2 s3(point_2(0.0, 0.0), point_2(0.0, 10.0));
    segment_2 s4(point_2(10.0, 0.0), point_2(10.0, 10.0));

    std::vector<segment_2> a;
    a.push_back(s1);
    a.push_back(s2);
    a.push_back(s3);
    a.push_back(s4);

    EXPECT_EQ(cg::segments_intersection(a).size(), 4);
}

TEST(simple, test4)
{
    using cg::point_2;
    using cg::segment_2;

    segment_2 s1(point_2(0.0, 0.0), point_2(10.0, 0.0));
    segment_2 s2(point_2(0.0, 10.0), point_2(10.0, 10.0));
    segment_2 s3(point_2(0.0, 0.0), point_2(0.0, 10.0));
    segment_2 s4(point_2(10.0, 0.0), point_2(10.0, 10.0));
    segment_2 s5(point_2(0.0, 0.0), point_2(10.0, 10.0));
    segment_2 s6(point_2(0.0, 10.0), point_2(10.0, 0.0));

    std::vector<segment_2> a;
    a.push_back(s1);
    a.push_back(s2);
    a.push_back(s3);
    a.push_back(s4);
    a.push_back(s5);
    a.push_back(s6);

    EXPECT_EQ(cg::segments_intersection(a).size(), 5);
}

TEST(simple, test5)
{
    using cg::point_2;
    using cg::segment_2;

    segment_2 s1(point_2(0.0, 0.0), point_2(10.0, 0.0));
    segment_2 s2(point_2(0.0, 10.0), point_2(10.0, 10.0));
    segment_2 s3(point_2(0.0, 0.0), point_2(0.0, 10.0));
    segment_2 s4(point_2(10.0, 0.0), point_2(10.0, 10.0));
    segment_2 s5(point_2(0.0, 0.0), point_2(10.0, 10.0));
    segment_2 s6(point_2(0.0, 10.0), point_2(10.0, 0.0));
    segment_2 s7(point_2(10.0, 5.0), point_2(0.0, 5.0));

    std::vector<segment_2> a;
    a.push_back(s1);
    a.push_back(s2);
    a.push_back(s3);
    a.push_back(s4);
    a.push_back(s5);
    a.push_back(s6);
    a.push_back(s7);

    EXPECT_EQ(cg::segments_intersection(a).size(), 7);
}

TEST(simple, test6)
{
    using cg::point_2;
    using cg::segment_2;

    segment_2 s1(point_2(0.0, 0.0), point_2(10.0, 0.0));
    segment_2 s2(point_2(0.0, 10.0), point_2(10.0, -10.0));
    segment_2 s3(point_2(0.0, -5.0), point_2(5.0, 10.0));

    std::vector<segment_2> a;
    a.push_back(s1);
    a.push_back(s2);
    a.push_back(s3);

    EXPECT_EQ(cg::segments_intersection(a).size(), 3);
}

TEST(simple, test7)
{
    using cg::point_2;
    using cg::segment_2;

    segment_2 s1(point_2(0.0, 0.0), point_2(10.0, 0.0));
    segment_2 s2(point_2(0.0, 5.0), point_2(10.0, 5.0));
    segment_2 s3(point_2(0.0, 7.0), point_2(7.0, 12.0));

    std::vector<segment_2> a;
    a.push_back(s1);
    a.push_back(s2);
    a.push_back(s3);

    EXPECT_EQ(cg::segments_intersection(a).size(), 0);
}


