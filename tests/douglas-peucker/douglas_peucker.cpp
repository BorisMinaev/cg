#include <gtest/gtest.h>

#include <cg/algo/douglas-peucker.h>

using namespace std;
using namespace cg;


TEST(simple, test1)
{
   vector<point_2> vec;
   vec.push_back(point_2(0.0, 0.0));
   vec.push_back(point_2(4.0, 4.1));
   vec.push_back(point_2(10.0, 10.0));
   vector<point_2> ans;
   douglas_peucker(vec.begin(), vec.end(), back_inserter(ans), 2.0);
   ASSERT_EQ(ans.size(), 2);
}

TEST(simple, test2)
{
   vector<point_2> vec;
   vec.push_back(point_2(0.0, 0.0));
   vec.push_back(point_2(4.0, 4.1));
   vec.push_back(point_2(10.0, 10.0));
   vector<point_2> ans;
   douglas_peucker(vec.begin(), vec.end(), back_inserter(ans), 0.0);
   ASSERT_EQ(ans.size(), 3);
}

TEST(simple, test3)
{
   vector<point_2> vec;
   vec.push_back(point_2(0.0, 0.0));
   vec.push_back(point_2(-4.0, -4.1));
   vec.push_back(point_2(10.0, 10.0));
   vector<point_2> ans;
   douglas_peucker(vec.begin(), vec.end(), back_inserter(ans), 2.0);
   ASSERT_EQ(ans.size(), 3);
}

TEST(simple, test4)
{
   vector<point_2> vec;
   vec.push_back(point_2(0.0, 0.0));
   vector<point_2> ans;
   douglas_peucker(vec.begin(), vec.end(), back_inserter(ans), 2.0);
   ASSERT_EQ(ans.size(), 1);
}
