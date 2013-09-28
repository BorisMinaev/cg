#include <QColor>
#include <QApplication>

#include <boost/optional.hpp>

#include <cg/visualization/viewer_adapter.h>
#include <cg/visualization/draw_util.h>

#include <cg/io/point.h>

#include <cg/primitives/point.h>
#include <cg/algo/douglas_peucker.h>
#include <cmath>

using cg::point_2f;
using cg::point_2;

struct douglas_viewer : cg::visualization::viewer_adapter
{
   douglas_viewer()
      : eps(5.0)
   {}

   void draw(cg::visualization::drawer_type & drawer) const
   {
      drawer.set_color(Qt::white);
      for (point_2 const & p : pts_)
         drawer.draw_point(p);
      drawer.set_color(Qt::green);
      for (size_t i = 0; i + 1 < res.size(); i++)
      {
         drawer.draw_line(res[i], res[i + 1]);
      }
   }

   void print(cg::visualization::printer_type & p) const
   {
      p.corner_stream() << "press mouse rbutton to add point" << cg::visualization::endl
                        << "points: " << pts_.size() << cg::visualization::endl
                        << "eps = " << eps << cg::visualization::endl
                        << "press 'right' & 'left' to change eps" << cg::visualization::endl;
   }

   void build() {
      res.clear();
      cg::douglas_peucker(pts_.begin(), pts_.end(), back_inserter(res), eps);
   }

   bool on_release(const point_2f & p)
   {
      pts_.push_back(p);
      build();
      return true;
   }

   bool on_key(int key)
   {
      switch (key)
      {
      case Qt::Key_Right : eps = eps + 5; break;
      case Qt::Key_Left : eps = std::max(eps - 5, 0.0); break;
      default : return false;
      }

      build();
      return true;
   }

private:
   std::vector<point_2> pts_;
   std::vector<point_2> res;
   double eps;
};

int main(int argc, char ** argv)
{
   QApplication app(argc, argv);
   douglas_viewer viewer;
   cg::visualization::run_viewer(&viewer, "douglas_peucker");
}
