#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "object.h"

/*
    Class created for the course Computer graphics (2016 - 2017).
    This represents a triangle
*/

class Triangle : public Object
{
    public:
        Triangle(Point v0, Point v1, Point v2);

        virtual Hit intersect(const Ray &ray);

        const Point v0;
        const Point v1;
        const Point v2;

        //these contstanst prevent constant recalculation
        //const Vector edge0;
        //const Vector edge1;
        //const Vector edge2;
        //const Vector N;
};

#endif