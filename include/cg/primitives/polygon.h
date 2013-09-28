#pragma once

#include <vector>

#include "contour.h"

namespace cg
{
   template <class Scalar>
   struct polygon_2t;

   typedef polygon_2t<float> polygon_2f;
   typedef polygon_2t<double> polygon_2;
   typedef polygon_2t<int>   polygon_2i;

   template <class Scalar>
   struct polygon_2t
   {
      polygon_2t(std::vector<contour_2t<Scalar> > const& cntrs) : cntrs_(cntrs)
      {}

      typedef typename std::vector<contour_2t<Scalar> >::const_iterator const_iterator;

      const_iterator begin() const
      {
         return cntrs_.begin();
      }

      const_iterator end() const
      {
         return cntrs_.end();
      }

      size_t size() const
      {
         return cntrs_.size();
      }

      void add_contour(contour_2t<Scalar const& contour)
      {
         cntrs_.push_back(contour);
      }

      contour_2t<Scalar> const& operator [] (size_t idx) const
      {
         return cntrs_[idx];
      }

      contour_2t<Scalar> & operator [] (size_t idx)
      {
         return cntrs_[idx];
      }

   private:
      std::vector <contour_2t <Scalar> > cntrs_;
   };
}
