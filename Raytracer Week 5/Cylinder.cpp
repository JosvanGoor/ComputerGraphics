#include "Cylinder.h"
#include <cmath>

Cylinder::Cylinder(Point p, Vector v, double r, double l)
    : start(p), V(v.normalized()), radius(r), length(l) {}

Hit Cylinder::intersect(const Ray &ray)
{
    Vector X = ray.O - start;

    double a = ray.D.dot(ray.D) - std::pow(ray.D.dot(V), 2);
    double b = ray.D.dot(X) - ray.D.dot(V) * X.dot(V);
    double c = X.dot(X) - std::pow(X.dot(V), 2) - std::pow(radius, 2);

    double d = b * b - a * c;
    if(d < 0) return Hit::NO_HIT();

    d = sqrt(d);
    double t1 = -b - d / a;
    double t2 = -b + d / a;

    //Choose the nearest point
    double t;
    if (t1 < t2) t = t1; else t = t2;
    if(t < 0.0) return Hit::NO_HIT(); //behind camera

    double m = ray.D.dot(V) * t + X.dot(V);
    if(m > length || m < 0) return Hit::NO_HIT();

    Point P = ray.at(t);
    Vector N = (P - start - (V*m)).normalized();

    if(ray.D.dot(N) > 0) N = -N;
    return Hit(t, N, this);
}

Color Cylinder::colorAt(const Point &point)
{
    return material->color;
}
