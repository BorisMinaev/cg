#pragma once

#include <cg/primitives/segment.h>
#include <cg/io/point.h>
#include <cg/operations/has_intersection/segment_segment.h>
#include <cg/operations/orientation.h>
#include <cg/convex_hull/graham.h>
#include <cmath>
#include <map>

namespace cg
{
template<class T>
bool has_intersection_without_ends(segment_2t<T> const & s1, segment_2t<T> const & s2) {
    if (!has_intersection(s1, s2))
        return false;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            if (s1[i] == s2[j])
                return false;
    return true;
}

    template<class T>
bool has_intersection2(segment_2t<T> const & seg, std::vector< contour_2t <T> > const & g) {
    for (auto g_it = g.begin(); g_it != g.end(); g_it++) {
        for (auto pt_it = g_it->begin(); pt_it != g_it->end(); pt_it++) {
            auto next = pt_it+1;
            if (next == g_it->end())
                next = g_it->begin();
            segment_2t<T> seg_contour(*pt_it, *next);
            if (has_intersection_without_ends(seg, seg_contour)) {
                return true;
            }
        }
    }
    return false;
}

template<class T>
void add_all_segments(contour_2t<T> const & cont, std::vector< contour_2t <T> > const & g, std::back_insert_iterator< typename std::vector< segment_2t<T> > > out) {

    for (auto it1 = cont.begin(); it1 != cont.end(); it1++) {
        for (auto it2 = cont.begin(); it2 != cont.end(); it2++) {
            if (has_intersection2(segment_2t<T>(*it1, *it2), g))
                continue;
            auto it1prev = (it1 == cont.begin() ? cont.end() - 1 : it1 - 1);
            auto it1next = (it1 == cont.end() - 1 ? cont.begin() : it1 + 1);
            orientation_t o1 = orientation(*it1prev, *it1, *it1next);
            orientation_t o2 = orientation(*it1prev, *it1, *it2);
            orientation_t o3 = orientation(*it1, *it1next, *it2);
            if (o1 == CG_LEFT) {
                if (o2 != CG_LEFT || o3 != CG_LEFT) {
                    *out++ = segment_2t<T> (*it1, *it2);
                }
            } else {
                if (o2 != CG_LEFT && o3 != CG_LEFT) {
                    *out++ = segment_2t<T> (*it1, *it2);
                }
            }
        }
    }
}

template<class T>
bool need_remove(point_2t<T> const & o, contour_2t<T> const & cont, typename contour_2t<T>::const_iterator & iter) {
    auto prev = iter == cont.begin() ? cont.end() - 1 : iter -1;
    auto next = iter == cont.end() - 1 ? cont.begin() : iter + 1;
    orientation_t o1 = orientation(*prev, *iter, o);
    orientation_t o2 = orientation(*iter, *next, o);
    return (o1 == CG_RIGHT && o2 == CG_RIGHT);
}

template<class T>
std::vector< segment_2t<T> > make_visibility_graph(point_2t<T> const & s, point_2t<T> const & f, std::vector< contour_2t <T> > const & g)
{
    std::vector<segment_2t<T> > result;
    for (auto it = g.begin(); it != g.end(); it++) {
        add_all_segments(*it, g, back_inserter(result));
    }
    for (auto it1 = g.begin(); it1 != g.end(); it1++)
        for (auto it2 = g.begin(); it2 != g.end(); it2++)
            if (it1 != it2) {
                for (auto it1p = it1->begin(); it1p != it1->end(); it1p++)
                    for (auto it2p = it2->begin(); it2p != it2->end(); it2p++)
                        if (!has_intersection2(segment_2t<T>(*it1p, *it2p), g))
                            result.push_back(segment_2t<T>(*it1p, *it2p));
            }
    for (auto it1 = g.begin(); it1 != g.end(); it1++)
        for (auto it1p = it1->begin(); it1p != it1->end(); it1p++) {
            if (!has_intersection2(segment_2t<T>(s, *it1p), g))
                result.push_back(segment_2t<T>(s, *it1p));
            if (!has_intersection2(segment_2t<T>(f, *it1p), g))
                result.push_back(segment_2t<T>(f, *it1p));
        }
    if (!has_intersection2(segment_2t<T>(s, f), g)) {
        result.push_back(segment_2t<T>(s, f));
    }
    std::vector<segment_2t<T> > result2;
    for (auto seg = result.begin(); seg != result.end(); seg++) {
        point_2t<T> start = (*seg)[0];
        point_2t<T> end = (*seg)[1];
        bool can_remove = false;
        for (auto it = g.begin(); it != g.end(); it++)
            for (auto itp = it->begin(); itp != it->end(); itp++){
                if ((*itp) == start) {
                    if (need_remove(end, *it, itp))
                        can_remove = true;
                }
                if ((*itp) == end) {
                    if (need_remove(start, *it, itp))
                        can_remove = true;
                }
            }
        if (!can_remove)
            result2.push_back(*seg);
    }
    return result2;
}

template<class T>
std::vector< segment_2t<T> > dejkstra(point_2t<T> const & s, point_2t<T> const & f, std::vector< segment_2t<T> > const & g)
{
    std::vector<segment_2t<T> > result;
    std::map<point_2t<T>, int> to_id;
    to_id.insert(std::make_pair(s, 0));
    to_id.insert(std::make_pair(f, 1));
    int size = 2;
    std::vector<point_2t<T>> all_points;
    all_points.push_back(s);
    all_points.push_back(f);
    for (auto seg = g.begin(); seg != g.end(); seg++) {
        point_2t<T> pt1 = (*seg)[0];
        point_2t<T> pt2 = (*seg)[1];
        if (!to_id.count(pt1)) {
            to_id.insert(std::make_pair(pt1, size++));
            all_points.push_back(pt1);
        }
        if (!to_id.count(pt2)) {
            to_id.insert(std::make_pair(pt2, size++));
            all_points.push_back(pt2);
        }
    }
    std::vector<std::vector<std::pair<int, double> > > graph;
    for (int i = 0; i < size; i++)  {
        std::vector<std::pair<int, double> > tmp;
        graph.push_back(tmp);
    }
    for (auto seg = g.begin(); seg != g.end(); seg++) {
        point_2t<T> pt1 = (*seg)[0];
        point_2t<T> pt2 = (*seg)[1];
        int id1 = to_id[pt1];
        int id2 = to_id[pt2];
        double dx = pt1.x - pt2.x;
        double dy = pt1.y - pt2.y;
        double dist = sqrt(dx*dx+dy*dy);
        graph[id1].push_back(std::make_pair(id2, dist));
        graph[id2].push_back(std::make_pair(id1, dist));
    }
    std::vector<double> dist(size, INFINITY);
    dist[0] = 0;
    std::vector<bool> was(size, false);
    std::vector<int> from(size, -1);
    for (int i = 0; i < size; i++) {
        double best_dist = INFINITY;
        int best_id = -1;
        for (int j = 0; j < size; j++) {
            if (!was[j] && dist[j] < best_dist) {
                best_id = j;
                best_dist = dist[j];
            }
        }
            if (best_id == -1)
                break;
            was[best_id] = true;
            for (int j = 0; j < graph[best_id].size(); j++) {
                int to = graph[best_id][j].first;
                double d2 = graph[best_id][j].second + dist[best_id];
                if (d2 < dist[to]) {
                    dist[to] = d2;
                    from[to] = best_id;
                }
            }
    }
    std::vector<int> path;
    int cur = 1;
    path.push_back(1);
    while (cur != 0) {
        cur = from[cur];
        path.push_back(cur);
    }
    for (int i = path.size() - 1; i > 0; i--) {
        point_2t<T> pt1 = all_points[path[i]];
        point_2t<T> pt2 = all_points[path[i - 1]];
        result.push_back(segment_2t<T>(pt1, pt2));
    }
    return result;
}
}
