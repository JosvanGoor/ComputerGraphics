#include "Triangle.hpp"


Triangle::Triangle(Point v0, Point v1, Point v2)
    :   v0(v0), v1(v1), v2(v2) {}
        //edge0(v1 - v0), edge1(v2 - v0), edge2(v0 - v2), 
        //N(edge0.cross(edge1).normalized()) {}

//http://www.lighthouse3d.com/tutorials/maths/ray-triangle-intersection/

Hit Triangle::intersect(const Ray &ray)
{
    Vector e1 = v1 - v0;
    Vector e2 = v2 - v0;
    Vector e1e2 = e1.cross(e2).normalized();

    Vector p = ray.D.cross(e2);
    float a = e1.dot(p);
    if(a < 0.0001) return Hit::NO_HIT();

    float f =  1 / a;
    Vector s = ray.O - v0;
    float u = f*(s.dot(p));
    if(u < 0.0 || u > 1.0)
        return Hit::NO_HIT();

    Vector q = s.cross(e1);
    float v = f * (ray.D.dot(q));
    if(v < 0.0 || u + v > 1.0)
        return Hit::NO_HIT();

    float t = f * (e2.dot(q));

    return Hit(t, e1e2, this);
}

Color Triangle::colorAt(const Point &point)
{
    return material->color;
}