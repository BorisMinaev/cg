#include <QColor>
#include <QApplication>

#include <boost/optional.hpp>

#include <cg/visualization/viewer_adapter.h>
#include <cg/visualization/draw_util.h>

#include <cg/io/point.h>

#include <cg/primitives/point.h>
#include <cg/primitives/segment.h>
#include <cg/primitives/triangle.h>

#include <cg/algo/intersect_sphere.h>
#include <random>

using cg::point_3;
using cg::point_2;
using cg::point_2f;
using cg::triangle_2;
using cg::segment_2;

double PI = asin(1.0) * 2.0;

struct intersection_viewer : cg::visualization::viewer_adapter
{
    inline double random_double()
    {
        return unif(re);
    }

    void testA() {
//        p1 = point_3(0.0, 0.0);
//        p2 = point_3(0.0, PI / 2);

//        p3 = point_3(-PI/4, 0);
//        p4 = point_3(PI/4, 0);

        p1 = point_3(0.0, -PI/4);
        p2 = point_3(0.0, PI / 4);

        p3 = point_3(-PI/4, PI);
        p4 = point_3(PI/4, PI);
    }

    void testB() {
        p1 = point_3(0.0, 0.0);
        p2 = point_3(0.0, PI / 2);

        p3 = point_3(-PI/4, PI / 2);
        p4 = point_3(PI/4, PI/4);
    }

    void testC() {
        p1 = point_3(PI / 2, 0.0);
        p2 = point_3(0.0, PI / 2);

        p3 = point_3(-PI/4, PI / 2);
        p4 = point_3(PI/4, PI/4);
    }

    void testD() {
        p1 = point_3(-PI * 3 / 4, 0.0);
        p2 = point_3(PI * 3/ 4, 3*PI/4);

        p3 = point_3(-PI*3/4, 3*PI/4);
        p4 = point_3(PI*3/ 4, 0.0);
    }

    point_3 rand_pt() {
        double x = (random_double()-0.5) * PI / 2;
        double y = random_double() * PI * 2;
        return point_3(x, y);
    }

    void testE() {

        p1 = rand_pt();
        p2 = rand_pt();

        p3 = rand_pt();
        p4 = rand_pt();
    }

   intersection_viewer()
   {
       re.seed(123);
       testA();
   }

   std::vector<point_3> get_betw(const point_3 & a, const point_3 & b) const {
    std::vector<point_3> result;
    const int ITER_COUNT = 360;
    for (int it = 0; it < ITER_COUNT; it++){
        double x = a.x + (b.x - a.x) * it / (ITER_COUNT - 1);
        double y = a.y + (b.y - a.y) * it / (ITER_COUNT - 1);
        double z = a.z + (b.z - a.z) * it / (ITER_COUNT - 1);
        point_3 res(x, y, z);
        res.normalize();
        result.push_back(res);
    }
    return result;
   }

   point_2 to2d(point_3 & a) const {
       double cosA = cos(a1), sinA = sin(a1);
       double cosB = cos(a2), sinB = sin(a2);
       double cosC = cos(a3), sinC = sin(a3);
       double x = a.x * (cosA * cosC - sinA * cosB * sinC) + a.y * (-cosA*sinC-sinA*cosB*cosC) + a.z * (sinA * sinB);
       double y = a.x * (sinA * cosC + cosA * cosB * sinC) + a.y * (-sinA*sinC+cosA*cosB*cosC) + a.z * (-cosA * sinB);
       return point_2(x*100, y*100);
   }

   void my_draw_point(cg::visualization::drawer_type & drawer, point_3 a, int r) const {
      drawer.draw_point(to2d(a), r);
   }

   void draw_seg(cg::visualization::drawer_type & drawer, point_3 & a, point_3 & b) const {
       point_2 a1 = to2d(a);
       point_2 b1 = to2d(b);
       drawer.draw_line(a1, b1, 1);
   }

   void draw_line(cg::visualization::drawer_type & drawer, const point_3 & a, const point_3 & b) const {
    std::vector<point_3> pts = get_betw(a, b);
    drawer.set_color(Qt::red);
    my_draw_point(drawer, a, 3);
    my_draw_point(drawer, b, 3);
    for (size_t i = 0; i + 1 < pts.size(); i++) {
        draw_seg(drawer, pts[i], pts[i + 1]);
    }
   }

   void draw(cg::visualization::drawer_type & drawer) const
   {
       drawer.set_color(Qt::green);
       int COUNT = 20;
       for (int it = 0; it < COUNT; it++) {
           for (int it2 = 0; it2 < COUNT; it2++) {
               point_3 a((it2 - COUNT / 2) * PI / COUNT, it * PI * 2 / COUNT);
               my_draw_point(drawer, a, 1);
           }
       }
       draw_line(drawer, p1, p2);
       draw_line(drawer, p3, p4);
       boost::optional<point_3> inter = intersection(p1, p2, p3, p4);
       if (inter) {
           drawer.set_color(Qt::yellow);
           my_draw_point(drawer, *inter, 5);
       }
   }

   void print(cg::visualization::printer_type & p) const
   {
      p.corner_stream() << "click 1, 2, 3, 4, 5, 6 to rotate" << cg::visualization::endl;
      p.corner_stream() << "click q, w, e, r to see examples" << cg::visualization::endl;
      p.corner_stream() << "click t to see random example" << cg::visualization::endl;
   }

   bool on_release(const point_2f & p)
   {
      return true;
   }

   bool on_key(int key)
   {
       double dd = 0.1;
       if (key == Qt::Key_1) {
           a1 -= dd;
           return true;
       }
       if (key == Qt::Key_2) {
           a1 += dd;
           return true;
       }
       if (key == Qt::Key_3) {
           a2 -= dd;
           return true;
       }
       if (key == Qt::Key_4) {
           a2 += dd;
           return true;
       }
       if (key == Qt::Key_5) {
           a3 -= dd;
           return true;
       }
       if (key == Qt::Key_6) {
           a3 += dd;
           return true;
       }
       if (key== Qt::Key_Q) {
           testA();
           return true;
       }
       if (key== Qt::Key_W) {
           testB();
           return true;
       }
       if (key== Qt::Key_E) {
           testC();
           return true;
       }
       if (key== Qt::Key_R) {
           testD();
           return true;
       }
       if (key== Qt::Key_T) {
           testE();
           return true;
       }
      return true;
   }

private:
   double a1 = 0, a2 = 0, a3 = 0;
   point_3 p1, p2, p3, p4;
   std::uniform_real_distribution<double> unif;
   std::default_random_engine re;
};

int main(int argc, char ** argv)
{
   QApplication app(argc, argv);
   intersection_viewer viewer;
   cg::visualization::run_viewer(&viewer, "sphere intersection");
}
