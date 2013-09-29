#pragma once

#include <cg/primitives/segment.h>
#include <cg/algo/segment_intersect.h>
#include <cg/operations/orientation.h>
#include <boost/numeric/interval.hpp>
#include <cmath>
#include <gmpxx.h>
#include <set>
#include <fstream>

namespace cg
{
typedef boost::numeric::interval_lib::unprotect<boost::numeric::interval<double> >::type interval;

struct new_line {
    segment_2 s;
    interval A, B, C;

    new_line(segment_2 s) : s(s) {
        A = interval(s[1].y - s[0].y);
        B = interval(s[0].x - s[1].x);
        C = -(s[0].x * A + s[0].y * B);
    }
};

struct new_line_mpq {
    mpq_class A, B, C;

    new_line_mpq(new_line l) {
        A = mpq_class(l.s[1].y) - l.s[0].y;
        B = mpq_class(l.s[0].x) - l.s[1].x;
        C = -mpq_class(l.s[0].x) * A - mpq_class(l.s[0].y) * B;
    }
};

point_2t<interval> intersection(new_line l1, new_line l2) {
    interval x = (l2.C * l1.B - l1.C * l2.B) / (l1.A * l2.B - l2.A * l1.B);
    interval y = (l1.C * l2.A - l2.C * l1.A) / (l1.A * l2.B - l2.A * l1.B);
    return point_2t<interval>(x, y);
}

point_2t<mpq_class> intersection_mpq(new_line l_1, new_line l_2) {
    new_line_mpq l1(l_1);
    new_line_mpq l2(l_2);
    mpq_class x = (l2.C * l1.B - l1.C * l2.B) / (l1.A * l2.B - l2.A * l1.B);
    mpq_class y = (l1.C * l2.A - l2.C * l1.A) / (l1.A * l2.B - l2.A * l1.B);
    return point_2t<mpq_class>(x, y);
}

struct new_point {
    double x, y;
    bool is_precise;
    segment_2 s1, s2;
    point_2t<interval> interval_value;

    new_point() {}

    new_point(double x, double y) : x(x), y(y), is_precise(true) {
        interval_value = point_2t<interval>(interval(x), interval(y));
    }
    new_point(segment_2 s1, segment_2 s2) : is_precise(false), s1(s1), s2(s2) {
        interval_value = intersection(new_line(s1), new_line(s2));
    }
    new_point(point_2 p) : is_precise(true) {
        x = p.x;
        y = p.y;
    }
};

std::ostream & operator << (std::ostream & out, new_point const & p) {
    out.precision(6);
    double x = p.is_precise ? p.x : ((p.interval_value.x.lower() + p.interval_value.x.upper())/2);
    double y = p.is_precise ? p.y : ((p.interval_value.y.lower() + p.interval_value.y.upper())/2);
    out << x << " " << y;
    return out;
}

std::ostream & operator << (std::ostream & out, new_line const & p) {
    out.precision(6);
    out << p.s[0].x<< " " << p.s[0].y << " " <<p.s[1].x<<" " << p.s[1].y << std::endl;
    return out;
}

bool less_rational(new_point const & pp1, new_point const & pp2) {
    point_2t<mpq_class> p1 = intersection_mpq(new_line(pp1.s1), new_line(pp1.s2));
    point_2t<mpq_class> p2 = intersection_mpq(new_line(pp2.s1), new_line(pp2.s2));
    return (p1.x < p2.x) || (p1.x == p2.x && p1.y < p2.y);
}

bool less_exect_rational(new_point const & pp1, new_point const & pp2) {
    point_2t<mpq_class> p1(mpq_class(pp1.x), mpq_class(pp1.y));
    point_2t<mpq_class> p2 = intersection_mpq(new_line(pp2.s1), new_line(pp2.s2));
    return (p1.x < p2.x) || (p1.x == p2.x && p1.y < p2.y);
}

bool less_rational_exact(new_point const & pp1, new_point const & pp2) {
    point_2t<mpq_class> p1 = intersection_mpq(new_line(pp1.s1), new_line(pp1.s2));
    point_2t<mpq_class> p2(mpq_class(pp2.x), mpq_class(pp2.y));
    return (p1.x < p2.x) || (p1.x == p2.x && p1.y < p2.y);
}

bool mless(new_point const & p1, new_point const & p2) {
    if (p1.is_precise && p2.is_precise) {
        return (p1.x < p2.x) || (p1.x == p2.x && p1.y < p2.y);
    }
    if (!p1.is_precise && !p2.is_precise) {
        if (p1.interval_value.x.upper() < p2.interval_value.x.lower())
            return true;
        if (p1.interval_value.x.lower() > p2.interval_value.x.upper())
            return false;
        return less_rational(p1, p2);
    }
    if (p1.is_precise && !p2.is_precise) {
        if (p1.x < p2.interval_value.x.lower())
            return true;
        if (p1.x > p2.interval_value.x.upper())
            return false;
        return less_exect_rational(p1, p2);
    }
    if (p1.interval_value.x.upper() < p2.x)
        return true;
    if (p1.interval_value.x.lower() > p2.x)
        return false;
    return less_rational_exact(p1, p2);
}

inline bool operator < (new_point const & a, new_point const & b) {
    return mless(a, b);
}

inline bool operator == (new_point const & a, new_point const & b) {
    return !mless(a, b) && !mless(b, a);
}

inline bool operator != (new_point const & a, new_point const & b) {
    return !(a == b);
}

int vect_mul(new_point const & a, new_point const & b, new_point const & c) {
    interval dx1 = b.interval_value.x - a.interval_value.x;
    interval dy1 = b.interval_value.y - a.interval_value.y;
    interval dx2 = c.interval_value.x - a.interval_value.x;
    interval dy2 = c.interval_value.y - a.interval_value.y;
    interval int_vect_mul = dx1 * dy2 - dx2 * dy1;
    if (int_vect_mul.lower() > 0)
        return 1;
    if (int_vect_mul.upper() < 0)
        return -1;
    point_2t<mpq_class> pp1, pp2, pp3;
    if (a.is_precise)
        pp1 = point_2t<mpq_class>(mpq_class(a.x), mpq_class(a.y)); else
        pp1 = intersection_mpq(new_line(a.s1), new_line(a.s2));
    if (b.is_precise)
        pp2 = point_2t<mpq_class>(mpq_class(b.x), mpq_class(b.y)); else
        pp2 = intersection_mpq(new_line(b.s1), new_line(b.s2));
    if (c.is_precise)
        pp3 = point_2t<mpq_class>(mpq_class(c.x), mpq_class(c.y)); else
        pp3 = intersection_mpq(new_line(c.s1), new_line(c.s2));
    mpq_class mdx1 = pp2.x - pp1.x;
    mpq_class mdy1 = pp2.y - pp1.y;
    mpq_class mdx2 = pp3.x - pp1.x;
    mpq_class mdy2 = pp3.y - pp1.y;
    mpq_class mpq_vect_mul = mdx1 * mdy2 - mdx2 * mdy1;
    if (mpq_vect_mul > 0)
        return 1;
    if (mpq_vect_mul < 0)
        return -1;
    return 0;
}

bool mless(new_line l1, new_line l2, new_point cur_time) {
    int vec = vect_mul(new_point(min(l1.s).x, min(l1.s).y), new_point(max(l1.s).x, max(l1.s).y), new_point(min(l2.s).x, min(l2.s).y));
    int vec2 = vect_mul(new_point(min(l1.s).x, min(l1.s).y), new_point(max(l1.s).x, max(l1.s).y), new_point(max(l2.s).x, max(l2.s).y));
    if (vec == 0) {
        return vec2 > 0;
    }
    if (!segment_intersect(l1.s, l2.s)){
        if (min(l1.s) < min(l2.s)) {
            point_2 up(min(l2.s).x, min(l2.s).y + 1);
            new_point inter(segment_2(min(l2.s), up), l1.s);
            return inter < new_point(min(l2.s));
        } else {
            point_2 up(min(l1.s).x, min(l1.s).y + 1);
            new_point inter(segment_2(min(l1.s), up), l2.s);
            return new_point(min(l1.s)) < inter;
        }
    }
    new_point intersection_point(l1.s, l2.s);
    if (intersection_point != cur_time) {
        if (intersection_point < cur_time) {
            return vec <= 0;
        } else {
            return vec >= 0;
        }
    } else {
        return vec <= 0;
    }
}

enum EVENT_TYPE {NEW_SEGMENT, END_SEGMENT, SEGMENT_INTERSECTION};

struct event {
    new_point time;
    EVENT_TYPE type;
    segment_2 s1, s2;
    event(new_point time, EVENT_TYPE type, segment_2 s) : time(time), type(type), s1(s) {}
    event(new_point time, EVENT_TYPE type, segment_2 s1, segment_2 s2) : time(time), type(type), s1(s1), s2(s2) {}
};

typedef std::set <event, std::function< bool(const event &,const event &) > > event_type;
typedef std::set <new_line, std::function< bool(const new_line &,const new_line &) > > status_type;


bool cmp(segment_2 const & a, segment_2 const & b) {
    if (a[0] == b[0])
        return a[1] < b[1];
    return a[0] < b[0];
}

inline bool mless(event const & a, event const & b) {
    if (a.time == b.time) {
        if (a.type == b.type) {
            if (!cmp(a.s1, b.s1) && !cmp(b.s1, a.s1))
                return cmp(a.s2, b.s2);
            return cmp(a.s1, b.s1);
        }
        return a.type < b.type;
    }
    return mless(a.time, b.time);
}

bool add_to_status(status_type & status, event_type & events, new_point const & cur_time, new_line const & nl) {
    auto lower = status.lower_bound(nl);
    bool has_intersection = false;
    if (lower != status.begin()) {
        lower--;
        if (segment_intersect((*lower).s, nl.s)) {
            event e1(new_point((*lower).s, nl.s), SEGMENT_INTERSECTION, (*lower).s, nl.s);
            if (e1.time == cur_time) {
                has_intersection = true;
            } else {
                if (cur_time < e1.time) {
                    events.insert(e1);
                }
            }
        }
    }
    auto upper = status.upper_bound(nl);
    if (upper != status.end()) {
        if (segment_intersect((*upper).s, nl.s)) {
            event e1(new_point((*upper).s, nl.s), SEGMENT_INTERSECTION, (*upper).s, nl.s);
            if (e1.time == cur_time) {
                has_intersection = true;
            } else {
                if (cur_time < e1.time) {
                    events.insert(e1);
                }
            }
        }
    }
    status.insert(nl);
    return has_intersection;
}

void erase_from_status(status_type & status, event_type & events, new_point const & cur_time, new_line const & nl) {
    status.erase(nl);
    auto lower = status.lower_bound(nl);
    if (lower != status.begin()) {
        lower--;
        auto upper = status.upper_bound(nl);
        if (upper != status.end()) {
            if (segment_intersect((*lower).s, (*upper).s)) {
                event e1(new_point((*lower).s, (*upper).s), SEGMENT_INTERSECTION, (*lower).s, (*upper).s);
                if (cur_time < e1.time) {
                    events.insert(e1);
                }
            }
        }
    }
}

std::vector<point_2> segments_intersection(std::vector<segment_2> & segments) {
    std::vector<point_2> result;
    new_point cur_time;
    auto comp2 = [&](const event& lhs, const event& rhs)  { return mless(lhs, rhs); };
    auto comp = [&](const new_line& lhs, const new_line& rhs)  { return mless(lhs, rhs, cur_time); };
    auto status = status_type (comp);
    auto events = event_type (comp2);
    for (segment_2 seg : segments) {
        events.insert(event(new_point(min(seg).x, min(seg).y), NEW_SEGMENT, seg));
        events.insert(event(new_point(max(seg).x, max(seg).y), END_SEGMENT, seg));
    }
    while (events.size() > 0) {
        event cur_event = *events.begin();
        events.erase(cur_event);
        std::vector<event> current_time_events;
        current_time_events.push_back(cur_event);
        while (events.size() > 0) {
            event new_event = *events.begin();
            if (new_event.time == cur_event.time) {
                events.erase(new_event);
                current_time_events.push_back(new_event);
            } else {
                break;
            }
        }
        bool has_intersection = current_time_events.size() > 1;
        for (auto event : current_time_events) {
            if (event.type == END_SEGMENT) {
                erase_from_status(status, events, cur_event.time, new_line(event.s1));
                //status.erase(event.s1);
            } else {
                if (event.type == SEGMENT_INTERSECTION) {
                    status.erase(new_line(event.s1));
                    status.erase(new_line(event.s2));
                }
            }
        }
        cur_time = cur_event.time;
        for (auto cevent : current_time_events) {
            if (cevent.type != END_SEGMENT) {
                if (cevent.type == NEW_SEGMENT) {
                    has_intersection |= add_to_status(status, events, cur_time, new_line(cevent.s1));
                } else {
                    has_intersection = true;
                    if (cur_event.time < new_point(max(cevent.s1))) {
                        add_to_status(status, events, cur_time, new_line(cevent.s1));
                    }
                    if (cur_event.time < new_point(max(cevent.s2))) {
                        add_to_status(status, events, cur_time, new_line(cevent.s2));
                    }
                }
            }
        }
        if (has_intersection) {
            double x = 0.5 * (cur_event.time.interval_value.x.lower() + cur_event.time.interval_value.x.upper());
            double y = 0.5 * (cur_event.time.interval_value.y.lower() + cur_event.time.interval_value.y.upper());
            result.push_back(point_2(x, y));
        }
    }
    //std::cout << "My answer is: "<< std::endl;
    //for (auto  p : result)
     // std::cout << p.x << " " << p.y << std::endl;
    return result;
    }
}
