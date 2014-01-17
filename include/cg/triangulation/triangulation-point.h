#pragma once

#include <algorithm>
#include <vector>
#include <set>
#include <cg/primitives/point.h>
#include <cg/primitives/triangle.h>
#include <cg/operations/orientation.h>
#include <cg/operations/in_circle.h>
#include <cg/operations/compare_dist.h>
#include <cg/io/triangle.h>
#include <cg/io/point.h>
#include <cmath>
#include <random>

namespace cg {

struct FAV_face;

struct FAV_point {
    point_2 point;
    bool is_inf;
    FAV_face *first_face;
    bool deleted;
    int id_in_vector;
    FAV_point * same_point_in_another_layer;

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
    if (cnt_inf > 1)
        return false;
    if (cnt_inf == 0)
        return in_circle(a.point, b.point, c.point, d.point);
    if (c.is_inf)
        return orientation(a.point, b.point, d.point) == CG_LEFT;
    if (a.is_inf)
        return orientation(b.point, c.point, d.point) == CG_LEFT;
    if (b.is_inf)
        return orientation(c.point, a.point, d.point) == CG_LEFT;
    if (d.is_inf)
        return orientation(a.point,b.point, c.point) == CG_COLLINEAR;
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
        for (int i = 0; i < 3; i++) {
            if (orientation(*points[i], *points[(i+1)%3], p) == CG_RIGHT)
                return false;
            if (orientation(*points[i], *points[(i+1)%3], p) == CG_COLLINEAR) {
                double minX = fmin(points[i]->point.x, points[(i+1)%3]->point.x);
                double maxX = fmax(points[i]->point.x, points[(i+1)%3]->point.x);
                double minY = fmin(points[i]->point.y, points[(i+1)%3]->point.y);
                double maxY = fmax(points[i]->point.y, points[(i+1)%3]->point.y);
                if (p.point.x < minX || p.point.x > maxX)
                    return false;
                if (p.point.y < minY || p.point.y > maxY)
                    return false;
            }
        }
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

    int next(FAV_point * x) {
        for (int i = 0; i < 3; i++)
            if (points[i] == x)
                return (i+1)%3;
        return -1;
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
    std::vector<FAV_point*> points;
    std::vector<FAV_face*> faces;
    std::vector<FAV_face*> faces_want_to_delete;
    FAV_point* inf;
    FAV_face *f1, *f2;
    std::vector<FAV_point*> before_points;
    bool exist_faces;

    FAV() {
        exist_faces = false;
        inf = new FAV_point(true);
        points.push_back(inf);
        f1 = new FAV_face();
        f2 = new FAV_face();
    }

    ~FAV() {
        for (size_t i = 0; i< points.size(); i++) {
            delete points[i];
        }
        for (size_t i = 0; i< faces.size(); i++) {
            delete faces[i];
        }
    }

    FAV_point * find_nearest(point_2 p) {
        FAV_point * cl = points[0];
        for (size_t i = 1; i < points.size(); i++) {
            if (points[i]->deleted)
                continue;
            if (cl->deleted || cl->is_inf) {
                cl = points[i];
                continue;
            }
            if (points[i]->is_inf)
                continue;
            if (compare_dist(p, points[i]->point, p, cl->point))
                cl = points[i];
        }
        for (size_t i = 0; i < before_points.size(); i++) {
            if (cl->deleted || cl->is_inf) {
                cl = before_points[i];
                continue;
            }
            if (before_points[i]->is_inf)
                continue;
            if (compare_dist(p, before_points[i]->point, p, cl->point))
                cl = before_points[i];
        }
        return cl;
    }

    std::vector<FAV_point*> get_all_neighbors(FAV_point * p, std::vector<FAV_point*> & res) {
        FAV_face * face = p->first_face;
        int id = face->next(p);
        res.push_back(face->points[id]);
        while (true) {
            face = face->neighbors[id];
            id = face->next(p);
            if (face->points[id] == res[0])
                break;
            res.push_back(face->points[id]);
        }
        return res;
    }

    std::vector<FAV_face*> get_all_neighbor_triangles(FAV_point * p, std::vector<FAV_face*> & res) {
        FAV_face * face = p->first_face;
        int id = face->next(p);
        res.push_back(face);
        while (true) {
            face = face->neighbors[id];
            id = face->next(p);
            if (face == res[0])
                break;
            res.push_back(face);
        }
        return res;
    }

    void clean_points() {
        for (size_t i = 0; i < points.size(); i++) {
            if (points[i]->deleted) {
                delete points[i];
                points[i] = points[points.size() - 1];
                points[i]->id_in_vector = i;
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
        points[point->id_in_vector]->id_in_vector = point->id_in_vector;
        points.pop_back();
        delete point;
    }

    bool check_exist_faces(FAV_point * pt) {
        if (exist_faces)
            return true;
        before_points.push_back(pt);
        if (before_points.size() < 3) {
            return false;
        }
        if (orientation(before_points[0]->point, before_points[1]->point, pt->point) != CG_COLLINEAR) {
            add_new_point(before_points[0]);
            add_new_point(before_points[1]);
            check_first_state();
            exist_faces = true;
            for (size_t i = before_points.size() - 1; i > 1; i--) {
                add_new_point(before_points[i]);
                add_point_to_existing_FAV(*before_points[i]);
            }
            before_points.clear();
            return false;
        }
        return false;
    }

    void check_first_state() {
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
            add_new_face(f1);
            add_new_face(f2);
    }

    void update_flip(FAV_face *face) {
        if (face->deleted)
            return;
        for (int i = 0; i < 3; i++) {
            int opp_point = get_opposite_point(*(face->neighbors[i]), face);
            FAV_point * opp = face->neighbors[i]->points[opp_point];
            if (in_circle(*(face->points[i]), *(face->points[(i+1)%3]),*(face->points[(i+2)%3]), *opp) ) {
                FAV_face* face1 = face, *face2 = face->neighbors[i];
                f1->points[0] = face1->points[i]; f1->points[1] = face2->points[opp_point]; f1->points[2] = face1->points[(i+2)%3];
                f2->points[0] = face2->points[opp_point]; f2->points[1] = face1->points[i]; f2->points[2] = face1->points[(i+1)%3];
                f1->neighbors[0] = face2->neighbors[(opp_point+2)%3]; f1->neighbors[1] = face1->neighbors[(i+1)%3]; f1->neighbors[2] = face2;
                f2->neighbors[0] = face1->neighbors[(i+2)%3]; f2->neighbors[1] = face2->neighbors[(opp_point+1)%3]; f2->neighbors[2] = face1;
                for (int i = 0; i < 3; i++) {
                    face1->points[i] = f1->points[i];
                    face1->neighbors[i] = f1->neighbors[i];
                    face2->points[i] = f2->points[i];
                    face2->neighbors[i] = f2->neighbors[i];
                }
                for (int i = 0; i < 3; i++) {
                    face1->neighbors[i]->update_neighbor(face1);
                    face2->neighbors[i]->update_neighbor(face2);
                }
                face1->update_points();
                face2->update_points();
                update_flip(face1);
                update_flip(face2);
                break;
            }
        }
    }

    void add_point_to_face(FAV_point & p, FAV_face *ins) {
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
    }

    void add_point_to_existing_FAV(FAV_point & p) {
        for (size_t i = 0; i < faces.size(); i++)
            if (!faces[i]->deleted) {
                if (faces[i]->inside(p)) {
                    if (!faces[i]->contains_inf()) {
                        add_point_to_face(p, faces[i]);
                        return;
                    }
                }
            }
        for (size_t i = 0; i < faces.size(); i++)
            if (!faces[i]->deleted) {
                if (faces[i]->inside(p)) {
                    add_point_to_face(p, faces[i]);
                    return;
                }
            }
    }

    // [ab] inersects (cd)
    bool need_go(FAV_point * a, FAV_point * b, FAV_point * c, FAV_point * d) {
        if (a->is_inf)
            return orientation(*b, *c, *d)==CG_RIGHT;
        orientation_t f3 = orientation(*c, *d, *a);
        orientation_t f4 = orientation(*c, *d, *b);
        return f3 != f4;
    }

    FAV_face * find_face_contains_point(FAV_point * p, FAV_point * start) {
        FAV_face * res = start->first_face;
        while (!res->inside(p->point)) {
            bool found  = false;
            for (int i = 0; i < 3; i++) {
                if (need_go(res->points[i], p, res->points[(i+1)%3], res->points[(i+2)%3])) {
                    res = res->neighbors[i];
                    found = true;
                    break;
                }
            }
            if (!found )
                error("error localization.");
        }
        return res;
    }

    bool compare_dist2(point_2 & to, FAV_point * cur_best, FAV_point * new_p) {
        if (new_p->is_inf)
            return false;
        if (cur_best->is_inf)
            return true;
        return compare_dist(to, new_p->point, to, cur_best->point);
    }

    const bool find_nearest_point_approximatly = true;

    FAV_point * find_nearet_recursive(point_2 & p, FAV_point * cur_best, FAV_face * face) {
        int best_id = -1;
        for (int i = 0; i < 3; i++) {
            if (compare_dist2(p, cur_best, face->points[i])) {
                best_id = i;
                cur_best = face->points[i];
            }
        }
        if (best_id == -1 || find_nearest_point_approximatly)
            return cur_best;
        for (int j = 0; j < 3; j++)
            if (j != best_id) {
                FAV_point * new_best = find_nearet_recursive(p, cur_best, face->neighbors[j]);
                if (compare_dist2(p, cur_best, new_best)) {
                    cur_best = new_best;
                }
            }
        return cur_best;
    }

    FAV_point * find_nearest(point_2 & p, FAV_point * start_from) {
        if (!exist_faces)
            return find_nearest(p);
        FAV_point p_fav = FAV_point(p);
        FAV_point inf = FAV_point(true);
        FAV_face * face_contains_p = find_face_contains_point(&p_fav, start_from);
        return find_nearet_recursive(p, &inf, face_contains_p);
    }

    FAV_point * add_point(point_2 p) {
        FAV_point* new_point = new FAV_point(p);
        if (check_exist_faces(new_point)) {
            add_new_point(new_point);
            add_point_to_existing_FAV(*new_point);
        }
        return new_point;
    }

    FAV_point * add_point_near_some_existing_point(point_2 p, FAV_point * existing) {
        FAV_point* new_point = new FAV_point(p);
        if (check_exist_faces(new_point)) {
            add_new_point(new_point);
            FAV_face * face = find_face_contains_point(new_point, existing);
            add_point_to_face(*new_point, face);
        }
        return new_point;
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

struct SkipListTriangulation {
    std::vector<FAV*> layers;
    double COEF = 0.1;
    std::uniform_real_distribution<double> unif;
    std::default_random_engine re;

    void add_new_layer() {
        FAV *fav = new FAV();
        layers.push_back(fav);
    }

    SkipListTriangulation() {
        add_new_layer();
    }

    bool add_to_next_layer() {
        return unif(re) < COEF;
    }

    ~SkipListTriangulation() {
        for (size_t i = 0; i < layers.size(); i++) {
            delete layers[i];
        }
    }

    void localize(point_2 & p, std::vector<FAV_point*> & result) {
        FAV_point * res = layers[layers.size() - 1]->find_nearest(p);
        result[layers.size() - 1] = res;
        for (int i = (int)layers.size() - 2; i >= 0; i--) {
            res = res->same_point_in_another_layer;
            res = layers[i]->find_nearest(p, res);
            result[i] = res;
        }
    }

    void add_point(point_2 p) {
         bool added_new_layer = false;
        size_t layer_id = 0;
        std::vector<FAV_point*> nearest(layers.size());
        localize(p, nearest);
        FAV_point * last = layers[0]->add_point_near_some_existing_point(p, nearest[0]);
        while (add_to_next_layer() && !added_new_layer) {
            layer_id++;
            if (layer_id == layers.size()) {
                add_new_layer();
                added_new_layer = true;
                FAV_point*next = layers[layer_id]->add_point(p);
                next->same_point_in_another_layer = last;
            } else {
                FAV_point*next = layers[layer_id]->add_point_near_some_existing_point(p, nearest[layer_id]);
                next->same_point_in_another_layer = last;
                last = next;
            }
        }
    }

    void remove_point(point_2 p) {

    }

    std::vector<triangle_2> get_triangulation() {
        return layers[0]->get_triangulation();
    }
};


    template <class RandIter>
    std::vector<triangle_2> make_triangulation(RandIter begin, RandIter end) {
        SkipListTriangulation skipList;
        std::set<point_2> alr_exist;
        for (auto it = begin; it != end; it++) {
            point_2 cur = *it;
            if (alr_exist.find(cur) != alr_exist.end())
                continue;
            alr_exist.insert(cur);
            skipList.add_point(cur);
        }
        return skipList.get_triangulation();
    }
}
