#include <QColor>
#include <QApplication>

#include <boost/optional.hpp>

#include <cg/visualization/viewer_adapter.h>
#include <cg/visualization/draw_util.h>

#include <cg/io/point.h>

#include <cg/primitives/point.h>
#include <cg/primitives/segment.h>
#include <cg/algo/bentley-ottmann.h>
#include <cmath>

using cg::point_2f;
using cg::point_2;

struct bentley_ottmann_viewer : cg::visualization::viewer_adapter
{
    bentley_ottmann_viewer() : has_point(false)
   {}

   void draw(cg::visualization::drawer_type & drawer) const
   {
      drawer.set_color(Qt::white);
      for (size_t i = 0; i < segments.size(); i++)
      {
          drawer.draw_line(segments[i][0], segments[i][1]);
      }
      drawer.set_color(Qt::red);
      for (point_2 const & p : res)
        drawer.draw_point(p, 5);
      drawer.set_color(Qt::yellow);
      if (has_point) {
          drawer.draw_point(last, 3);
      }
   }

   void print(cg::visualization::printer_type & p) const
   {
      p.corner_stream() << "press mouse rbutton to add point" << cg::visualization::endl
                        << "intersection points count = " << res.size() << cg::visualization::endl;
   }

   void build() {
      res = cg::segments_intersection(segments);
   }

   bool on_release(const point_2f & p)
   {
       if (has_point) {
           has_point = false;
           segments.push_back(cg::segment_2(last, p));
       } else {
           last = p;
           has_point = true;
       }
      build();
      return true;
   }

   bool on_key(int key)
   {
      switch (key)
      {
      default : return false;
      }

      build();
      return true;
   }

private:
   std::vector<cg::segment_2> segments;
   std::vector<cg::point_2> res;
   bool has_point;
   point_2 last;
};

int main(int argc, char ** argv)
{
   QApplication app(argc, argv);
   bentley_ottmann_viewer viewer;
   cg::visualization::run_viewer(&viewer, "bentley ottmann");
}
