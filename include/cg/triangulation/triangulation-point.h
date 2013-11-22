#pragma once

#include <algorithm>
#include <vector>
#include <set>
#include <cg/primitives/point.h>
#include <cg/primitives/triangle.h>
#include <cg/operations/orientation.h>
#include <cg/operations/in_circle.h>
#include <cg/io/triangle.h>
#include <cg/io/point.h>

namespace cg {

struct FAV_face;

struct FAV_point {
    point_2 point;
    bool is_inf;
    FAV_face *first_face;
    bool deleted;
    int id_in_vector;

    FAV_point() {}
    FAV_point(bool inf) : is_inf(inf), deleted(false) {}
    FAV_point(point_2 point) : point(point), is_inf(false), deleted(false) {}
};

orientation_t orientation(FAV_point const & a, FAV_point const & b, FAV_point const & c) {
    if (a.is_inf || b.is_inf || c.is_inf)
        return CG_LEFT;
    return orientation(a.point, b.point, c.point);
}

bool in_circle(FAV_point const & a, FAV_point const & b, FAV_point const & c, FAV_point const & d) {
    int cnt_inf = 0;
    if (a.is_inf) cnt_inf++;
    if (b.is_inf) cnt_inf++;
    if (c.is_inf) cnt_inf++;
    if (d.is_inf) cnt_inf++;
    if (cnt_inf > 1 || d.is_inf)
        return false;
    if (cnt_inf == 0)
        return in_circle(a.point, b.point, c.point, d.point);
    if (c.is_inf)
        return orientation(a.point, b.point, d.point) == CG_LEFT;
    if (a.is_inf)
        return orientation(b.point, c.point, d.point) == CG_LEFT;
    if (b.is_inf)
        return orientation(c.point, a.point, d.point) == CG_LEFT;
    return false;
}

void error(std::string message) {
    std::cout << "ERROR: "<< message << std::endl;
    throw new std::runtime_error("");
}

std::ostream & operator << (std::ostream & out, FAV_point const & s)
{
    if (s.is_inf)
        out<<"inf"; else
        out << s.point;
   return out;
}

struct FAV_face {
    FAV_face* neighbors[3];
    FAV_point* points[3];
    bool deleted;
    int id_in_vector;

    void print() {
        std::cout << "face [" << *points[0] << ", " <<* points[1] << ", " << *points[2] << "]" << std::endl;
    }

    bool contains_inf() {
        return points[0]->is_inf || points[1]->is_inf || points[2]->is_inf;
    }

    triangle_2 get_triangle() {
        return triangle_2(points[0]->point, points[1]->point, points[2]->point);
    }

    bool inside(FAV_point const & p) {
        for (int i = 0; i < 3; i++)
            if (orientation(*points[i], *points[(i+1)%3], p) == CG_RIGHT)
                return false;
        return true;
    }

    bool has_point(FAV_point *p) {
        return points[0] == p || points[1] == p || points[2] == p;
    }

    void update_neighbor(FAV_face* to) {
        for (int i = 0; i < 3; i++)
            if (to->has_point(points[(i+1)%3]) && to->has_point(points[(i+2)%3]))
                neighbors[i] = to;
    }

    void update_points() {
        for (int i = 0; i < 3; i++)
            points[i]->first_face = this;
    }

    FAV_face(): deleted(false) {}
};

int get_opposite_point(FAV_face const & f, FAV_face * opposite) {
    for (int i = 0; i < 3; i++) {
        if (f.neighbors[i] == opposite)
            return i;
    }
    return -1;
}


// faces & vertices
struct FAV {
    size_t points_alive;
    std::vector<FAV_point*> points;
    std::vector<FAV_face*> faces;
    std::vector<FAV_face*> faces_want_to_delete;
    FAV_point* inf;

    FAV() {
        inf = new FAV_point(true);
        points.push_back(inf);
        points_alive = 1;
    }

    ~FAV() {
        for (size_t i = 0; i< points.size(); i++) {
            delete points[i];
        }
        for (size_t i = 0; i< faces.size(); i++) {
            delete faces[i];
        }
    }

    void clean_points() {
        for (size_t i = 0; i < points.size(); i++) {
            if (points[i]->deleted) {
                delete points[i];
                points[i] = points[points.size() - 1];
                points.pop_back();
            }
        }
    }

    void delete_face(FAV_face * face) {
        face->deleted = true;
        faces_want_to_delete.push_back(face);
    }

    void really_delete_faces() {
        for (size_t i = 0; i < faces_want_to_delete.size(); i++) {
            FAV_face * face = faces_want_to_delete[i];
            faces[face->id_in_vector] = faces[faces.size() - 1];
            faces[face->id_in_vector]->id_in_vector = face->id_in_vector;
            faces.pop_back();
            delete face;
        }
        faces_want_to_delete.clear();
    }

    void add_new_face(FAV_face * face) {
        faces.push_back(face);
        face->id_in_vector = faces.size() - 1;
        for (int i = 0; i <3; i++) {
            face->neighbors[i]->update_neighbor(face);
        }
        face->update_points();
    }

    void add_new_point(FAV_point * point) {
        points.push_back(point);
        point->id_in_vector = points.size() - 1;
    }

    void delete_point(FAV_point * point) {
        points[point->id_in_vector] = points[points.size() - 1];
        points.pop_back();
        delete point;
    }

    void check_first_state() {
        if (points_alive == 3) {
            faces.clear();
            clean_points();
            FAV_face *f1=new FAV_face(),* f2=new FAV_face();
            f1->points[0] = inf;
            f1->points[1] = points[2];
            f1->points[2] = points[1];
            f2->points[0] = inf;
            f2->points[1] = points[1];
            f2->points[2] = points[2];
            for (int i = 0; i < 3; i++) {
                f1->neighbors[i] = f2;
                f2->neighbors[i] = f1;
            }
            inf->first_face = f1;
            add_new_face(f1);
            add_new_face(f2);
        }
    }

    void update_flip(FAV_face *face) {
        if (face->deleted)
            return;
        for (int i = 0; i < 3; i++) {
            int opp_point = get_opposite_point(*(face->neighbors[i]), face);
            FAV_point * opp = face->neighbors[i]->points[opp_point];
            if (in_circle(*(face->points[i]), *(face->points[(i+1)%3]),*(face->points[(i+2)%3]), *opp) ||
                    in_circle(*opp, *(face->points[(i+2)%3]), *(face->points[(i+1)%3]),*(face->points[i]))) {
                FAV_face* f1=new FAV_face(),* f2=new FAV_face();
                FAV_face* face1 = face, *face2 = face->neighbors[i];
                f1->points[0] = face1->points[i]; f1->points[1] = face2->points[opp_point]; f1->points[2] = face1->points[(i+2)%3];
                f2->points[0] = face2->points[opp_point]; f2->points[1] = face1->points[i]; f2->points[2] = face1->points[(i+1)%3];
                f1->neighbors[0] = face2->neighbors[(opp_point+2)%3]; f1->neighbors[1] = face1->neighbors[(i+1)%3]; f1->neighbors[2] = f2;
                f2->neighbors[0] = face1->neighbors[(i+2)%3]; f2->neighbors[1] = face2->neighbors[(opp_point+1)%3]; f2->neighbors[2] = f1;
                add_new_face(f1);
                add_new_face(f2);
                delete_face(face1);
                delete_face(face2);
                update_flip(f1);
                update_flip(f2);
                break;
            }
        }
    }

    void add_point_to_existing_FAV(FAV_point & p) {
        for (size_t i = 0; i < faces.size(); i++)
            if (!faces[i]->deleted) {
                if (faces[i]->inside(p)) {
                    FAV_face* ins = faces[i];
                    ins->deleted = true;
                    FAV_face* f1=new FAV_face(),* f2=new FAV_face(),* f3=new FAV_face();
                    f1->points[0] = ins->points[0]; f1->points[1] = ins->points[1]; f1->points[2] = &p;
                    f2->points[0] = ins->points[1]; f2->points[1] = ins->points[2]; f2->points[2] = &p;
                    f3->points[0] = ins->points[2]; f3->points[1] = ins->points[0]; f3->points[2] = &p;
                    f1->neighbors[0] = f2; f1->neighbors[1] = f3; f1->neighbors[2] = ins->neighbors[2];
                    f2->neighbors[0] = f3; f2->neighbors[1] = f1; f2->neighbors[2] = ins->neighbors[0];
                    f3->neighbors[0] = f1; f3->neighbors[1] = f2; f3->neighbors[2] = ins->neighbors[1];
                    add_new_face(f1);
                    add_new_face(f2);
                    add_new_face(f3);
                    update_flip(f1);
                    update_flip(f2);
                    update_flip(f3);
                    really_delete_faces();
                    return;
                }
            }
        error("It can't be truth.");
    }

    void add_point(point_2 p) {
        points_alive++;
        FAV_point* new_point = new FAV_point(p);
        points.push_back(new_point);
        if (points_alive > 3) {
            add_point_to_existing_FAV(*new_point);
        } else {
            check_first_state();
        }
    }

    void remove_point(point_2 p) {
        // TODO: write code here!
    }

    std::vector<triangle_2> get_triangulation() {
        std::vector<triangle_2> result;
        for (size_t i = 0; i < faces.size(); i++)
            if (!faces[i]->deleted && !faces[i]->contains_inf()) {
                result.push_back(faces[i]->get_triangle());
            }
        return result;
    }
};


    template <class RandIter>
    std::vector<triangle_2> make_triangulation(RandIter begin, RandIter end) {
        FAV fav;
        std::set<point_2> alr_exist;
        for (auto it = begin; it != end; it++) {
            point_2 cur = *it;
            if (alr_exist.find(cur) != alr_exist.end())
                continue;
            alr_exist.insert(cur);
            fav.add_point(cur);
        }
        return fav.get_triangulation();
    }
}
