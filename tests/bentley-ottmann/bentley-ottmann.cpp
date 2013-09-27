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
