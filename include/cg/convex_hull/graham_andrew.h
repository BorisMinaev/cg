#pragma once

#include <boost/range/algorithm/sort.hpp>

#include <cg/operations/orientation.h>

namespace cg
{
   template <class BidIter>
   BidIter contour_graham_hull(BidIter p, BidIter q)
   {
      if (p == q)
         return p;

      BidIter b = p;

      BidIter pt = p++;

      if (p == q)
         return p;

      BidIter t = p++;

      if (p == q)
         return p;

      for (; p != q; )
      {
         switch (orientation(*pt, *t, *p))
         {
         case CG_LEFT:
            pt = t++;
            std::iter_swap(t, p++);
            break;
         case CG_RIGHT:
            t = pt--;
            break;
         case CG_COLLINEAR:
            std::iter_swap(t, p++);
         }
      }

      if (orientation(*pt, *t, *b) <= 0)
         --t;

      return ++t;
   }


   template <class BidIter>
   void print_p(BidIter p) {
      std::cout << (*p).x << " " << (*p).y << std::endl;
   }

   template <class BidIter>
   BidIter graham_andrew_hull(BidIter p, BidIter q)
   {
      if (p == q)
         return q;

      std::iter_swap(p, std::min_element(p, q));

      BidIter min = p;

      BidIter t = p++;

      if (p == q)
         return p;

      std::iter_swap(p, std::max_element(p, q));

      BidIter max = p;            

      BidIter bound = std::partition(++p, q, [min, max](point_2 const & a) {
         return orientation(*min, *max, a) <= 0;
      });

      std::iter_swap(max, bound - 1);

      std::sort(min, bound);
      std::sort(bound, q);
      std::reverse(bound, q);
      /*


            for (auto zz = min; zz != bound; zz++)
         print_p(zz);
      std::cout << std::endl;
      for (auto zz = bound; zz != q; zz++)
         print_p(zz);

*/

      //for (auto zz = min; zz != q; zz++) {
      //   print_p(zz);
     // }
      //BidIter pt = contour_graham_hull(min, q);
      //for (auto zz = min; zz != pt; zz++)
       //   print_p(zz);

      return contour_graham_hull(min, q);
   }
}
