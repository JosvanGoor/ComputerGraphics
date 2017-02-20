#ifndef DISK_HPP
#define DISK_HPP

#include "object.h"

/*
    Class created for the course Computer graphics (2016 -2017).
    This represents a disk-shape.
    When radius is set to 0 the disk represents an infinite plane.
*/

class Disk : public Object
{

public:
    Disk(Point pos, Vector normal, double radius = 0)
        : position(pos), N(normal.normalized()), radius(radius) {};

    virtual Hit intersect(const Ray &ray);

    const Point position;
    const Vector N;
    const double radius;
};

#endif
