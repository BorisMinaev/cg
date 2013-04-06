#pragma once

#include <stdexcept>
#include <boost/lexical_cast.hpp>

#include "point.h"

namespace cg
{
   template <class Scalar> struct my_rectangle_2t;
   typedef my_rectangle_2t<double> my_rectangle_2d;

   template <class Scalar>
   struct my_rectangle_2t
   {
      my_rectangle_2t() {}

      my_rectangle_2t(point_2t<Scalar> const & p1, point_2t<Scalar> const & p2, point_2t<Scalar> const & p3, point_2t<Scalar> const & p4)
         : p1(p1)
         , p2(p2)
         , p3(p3)
         , p4(p4)
      {}

      point_2t<Scalar> const & operator[] (size_t i) const
      {
         switch (i)
         {
         case 0: return p1;
         case 1: return p2;
         case 2: return p3;
         case 3: return p4;
         default:
            throw std::logic_error("invalid index: " + boost::lexical_cast<std::string>(i));
         }
      }

      point_2t<Scalar> & operator[] (size_t i)
      {
         switch (i)
         {
         case 0: return p1;
         case 1: return p2;
         case 2: return p3;
         case 3: return p4;
         default:
            throw std::logic_error("invalid index: " + boost::lexical_cast<std::string>(i));
         }
      }

   private:
      point_2t<Scalar> p1, p2, p3, p4;
   };

}
