#pragma once

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/ch_graham_andrew.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;

namespace cg
{
   template <class RandIter>
   RandIter cgal_andrew_hull(RandIter p, RandIter q)
   {
       /*typedef typename RandIter::value_type mtype;
       std::vector<Point_2> data;
       for (RandIter it = p; it != q; it++) {
           point_2 pt = point_2(0.0, 0.1);
           data.push_back(Point_2(pt.x, pt.y));
       }
       std::vector<Point_2> result;
       CGAL::ch_graham_andrew( data.begin(), data.end(), back_inserter(result) );
       //for (size_t i = 0; i < result.size(); i++) {
       //    *p = mtype(result[i].x, result[i].y);
       //    p++;
       //}*/
       return p;
   }
}
