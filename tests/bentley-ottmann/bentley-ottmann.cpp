#include <gtest/gtest.h>

#include <cg/algo/bentley-ottmann.h>

using namespace std;
using namespace cg;


TEST(simple, test2)
{
   vector<segment_2f> segments;
   segments.push_back(segment_2f(point_2f(0.0, 0.0), point_2f(10.0, 10.0)));
   segments.push_back(segment_2f(point_2f(0.0, 10.0), point_2f(10.0, 0.0)));
   vector<point_2f> result;
   result.push_back(point_2f(5.0, 5.0));
   vector<point_2f> res = cg::segments_intersection(segments);
   ASSERT_EQ(result, res);

}

TEST(less_function, test1) {
   new_point p1(0.0, 0.0);
   new_point p2(segment_2f(point_2f(-1.0, -1.0), point_2f(1.0, 1.0)), segment_2f(point_2f(-1.0, 1.0), point_2f(1.0, -1.0)));
   ASSERT_EQ(mless(p1, p2), false);
   ASSERT_EQ(mless(p2, p1), false);
   new_point p3(1.0, 1.0);
   ASSERT_EQ(mless(p2, p3), true);
   ASSERT_EQ(mless(p3, p2), false);
   new_point p4(0.0, 1.0);
   ASSERT_EQ(mless(p2, p4), true);
   new_point p5(0.0, -0.000001);
   ASSERT_EQ(mless(p2, p5), false);
   ASSERT_EQ(mless(p5, p2), true);  
}