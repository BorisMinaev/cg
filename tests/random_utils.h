#pragma once

#include <cg/primitives/point.h>
#include <random>

inline std::vector<cg::point_2> uniform_points(size_t count)
{
    std::vector<cg::point_2> res(count);

    double lower_bound = 0;
    double upper_bound = 10000;
    std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
    std::default_random_engine re;
    re.seed(time(NULL));

    for (size_t l = 0; l != count; ++l)
    {
        res[l].x = unif(re);
        res[l].y = unif(re);
    }

    return res;
}

inline std::vector<cg::point_2> uniform_points(size_t count, int seed)
{
    std::vector<cg::point_2> res(count);

    double lower_bound = 0;
    double upper_bound = 10000;
    std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
    std::default_random_engine re;
    re.seed(seed);

    for (size_t l = 0; l != count; ++l)
    {
        res[l].x = unif(re);
        res[l].y = unif(re);
    }

    return res;
}
