#pragma once

#include <cg/primitives/segment.h>
#include <cg/operations/orientation.h>
#include <cg/primitives/point.h>
#include <cmath>

namespace cg
{
   struct range_tree_y {
        point_2 pt;
        range_tree_y *left, *right;
        double div, max_div;

        range_tree_y(const std::vector<point_2> & pts) {
            if (pts.size() == 1) {
                pt = pts[0];
                div = max_div = pt.y;
                left = right = NULL;
            } else {
                size_t mid = pts.size() / 2;
                std::vector<point_2> left_pts;
                std::vector<point_2> right_pts;
                for (size_t i = 0; i < mid; i++) {
                    left_pts.push_back(pts[i]);
                }
                for (size_t i = mid; i < pts.size(); i++) {
                    right_pts.push_back(pts[i]);
                }
                left = new range_tree_y(left_pts);
                right = new range_tree_y(right_pts);
                div = left->max_div;
                max_div = right->max_div;
            }
        }

        ~range_tree_y() {
            if (left != NULL)
               left->range_tree_y::~range_tree_y();
            if (right != NULL)
               right->range_tree_y::~range_tree_y();
        }

        void get_pts(std::vector<point_2> & res, double y_min, double y_max) {
            if (left == NULL || right == NULL) {
                if (pt.y >= y_min && pt.y <= y_max)
                    res.push_back(pt);
                return;
            }
            if (div >= y_min)
                left->get_pts(res, y_min, y_max);
            if (div <= y_max)
                right->get_pts(res, y_min, y_max);
        }
   };

   bool cmp_y(point_2 p1, point_2 p2) {
       return (p1.y < p2.y || (p1.y == p2.y && p1.x < p2.x));
   }

   bool cmp_x(point_2 p1, point_2 p2) {
       return (p1.x < p2.x || (p1.x == p2.x && p1.y < p2.y));
   }

   struct range_tree {
       range_tree_y * tree_y;
       range_tree *left, *right;
       point_2 pt;
       double my_min_x, my_max_x;

       range_tree(std::vector<point_2> & pts) {
           std::vector<point_2> pts_y;
           for (size_t i = 0; i < pts.size(); i++)
               pts_y.push_back(pts[i]);
           std::sort(pts.begin(), pts.end(), cmp_x);
           std::sort(pts_y.begin(), pts_y.end(), cmp_y);
           tree_y = new range_tree_y(pts_y);
           if (pts.size() == 1) {
               pt = pts[0];
               my_min_x = my_max_x = pt.x;
               left = right = NULL;
               return;
           }
           size_t mid = pts.size() / 2;
           std::vector<point_2> left_pts;
           std::vector<point_2> right_pts;
           for (size_t i = 0; i < mid; i++) {
               left_pts.push_back(pts[i]);
           }
           for (size_t i = mid; i < pts.size(); i++) {
               right_pts.push_back(pts[i]);
           }
           left = new range_tree(left_pts);
           right = new range_tree(right_pts);
           my_min_x = left->my_min_x;
           my_max_x = right->my_max_x;
       }

       ~range_tree() {
           if (left != NULL)
              left->range_tree::~range_tree();
           if (right != NULL)
              right->range_tree::~range_tree();
           tree_y->range_tree_y::~range_tree_y();
       }

       void get_pts(std::vector<point_2> & res, double x_min, double x_max, double y_min, double y_max) {
           if (x_min > my_max_x)
               return;
           if (x_max < my_min_x)
               return;
           if (x_min <= my_min_x && x_max >= my_max_x) {
               tree_y->get_pts(res, y_min, y_max);
               return;
           }
           if (left == NULL)
               return;
            left->get_pts(res, x_min, x_max, y_min, y_max);
            right->get_pts(res, x_min, x_max, y_min, y_max);
       }
   };
}
