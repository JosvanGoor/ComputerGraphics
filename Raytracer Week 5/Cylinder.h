#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "object.h"
#include "Disk.h"

/*
  Class created for the course Computer graphics (2016 - 2017).
  This represents a cylinder.
*/

class Cylinder : public Object
{
public:
    Cylinder(Point start, Vector V, double radius, double length);

    virtual Hit intersect(const Ray &ray);
    virtual Color colorAt(const Point &point);

    const Point start; //starting point of the cylinder
    const Vector V; //cylinders axis.
    const double radius;
    const double length;
};


#endif
