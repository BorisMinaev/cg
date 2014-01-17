#include <gtest/gtest.h>

#include "random_utils.h"

#include <boost/assign/list_of.hpp>

#include <cg/primitives/point.h>
#include <cg/algo/range_tree.h>

TEST(range_tree, simple_test)
{
   using cg::point_2;
    using cg::range_tree;

    int seed = 787788;
    int lower_bound = 0, upper_bound = 10000;
    std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
    std::default_random_engine re;
    re.seed(seed * 134);

    for (size_t ITER = 0; ITER < 1000; ITER++) {
        if (ITER % 50 == 0)
            std::cout << "iter = " << ITER << std::endl;
        std::vector<point_2> pts = uniform_points(100, seed+ITER);
        range_tree tree(pts);
        for (size_t ask = 0; ask < 100; ask++) {
            double min_x = unif(re), max_x = unif(re), min_y = unif(re), max_y = unif(re);
            if (min_x > max_x)
                std::swap(min_x, max_x);
            if (min_y > max_y)
                std::swap(min_y, max_y);
            std::vector<point_2> pts_ans;
            tree.get_pts(pts_ans, min_x, max_x, min_y, max_y);
            std::vector<point_2> real_ans;
            for (size_t i = 0; i < pts.size(); i++)
                if (pts[i].x >= min_x && pts[i].x <= max_x)
                    if (pts[i].y >= min_y && pts[i].y <= max_y)
                        real_ans.push_back(pts[i]);
            std::sort(real_ans.begin(), real_ans.end());
            std::sort(pts_ans.begin(), pts_ans.end());
            ASSERT_EQ(pts_ans.size(), real_ans.size());
            for (size_t i = 0; i < real_ans.size(); i++) {
                ASSERT_EQ(real_ans[i], pts_ans[i]);
            }
        }
    }
}

TEST(range_tree, time_test)
{
    using cg::point_2;
    using cg::range_tree;

    int seed = 787788;
    int lower_bound = 0, upper_bound = 10000;
    std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
    std::default_random_engine re;
    re.seed(seed * 134);

    const int MAX = 50000;

        std::vector<point_2> pts = uniform_points(MAX, seed);
        range_tree tree(pts);
        for (size_t ask = 0; ask < 100; ask++) {
            double min_x = unif(re), max_x = unif(re), min_y = unif(re), max_y = unif(re);
            if (min_x > max_x)
                std::swap(min_x, max_x);
            if (min_y > max_y)
                std::swap(min_y, max_y);
            std::vector<point_2> pts_ans;
            tree.get_pts(pts_ans, min_x, max_x, min_y, max_y);
        }
}

