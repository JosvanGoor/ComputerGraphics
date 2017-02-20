#include "Disk.h"
#include <cmath>

Hit Disk::intersect(const Ray &ray)
{
    //check whether the ray and disk are parralel
    if(!N.dot(ray.D))
        return Hit::NO_HIT();

    double t = N.dot(position - ray.O) / N.dot(ray.D);

    if(t <= 1.0e-10)
        return Hit::NO_HIT();

    //check disk bounds.
    Point hit = ray.at(t);
    double dist = (hit - position).length();

    if(radius < dist && radius != 0)
        return Hit::NO_HIT();

    return Hit(t, -N);
}
