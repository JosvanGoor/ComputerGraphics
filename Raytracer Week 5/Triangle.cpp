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

    return Hit(t, e1e2);
}

/*Hit Triangle::intersect(const Ray &ray)
{
    Vector edge0 = v1 - v0;
    Vector edge1 = v2 - v0;
    Vector edge2 = v0 - v2;
    Vector N = edge0.cross(edge1).normalized();

    //check that ray and triangle aren't parrallel.
    if(N.dot(ray.D) < 0.0001) return Hit::NO_HIT();

    //compute distance origin - plane
    float d = N.dot(v0);
    float t = (N.dot(ray.O) + d) / N.dot(ray.O); //this - maybe needs to go, we'll see
    
    //behind eye
    if(t < 0.0) return Hit::NO_HIT();

    Vector C;
    Point hit = ray.at(t);

    //inside out test
    Vector P0 = hit - edge0;
    C = edge0.cross(P0);
    if(N.dot(C) < 0) return Hit::NO_HIT();

    Vector P1 = hit - edge1;
    C = edge1.cross(P1);
    if(N.dot(C) < 0) return Hit::NO_HIT();

    Vector P2 = hit - edge2;
    C = edge2.cross(P2);
    if(N.dot(C) < 0) return Hit::NO_HIT();


    return Hit(t, N);
}*/