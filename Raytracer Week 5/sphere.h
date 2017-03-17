//
//  Framework for a raytracer
//  File: sphere.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#ifndef SPHERE_H_115209AE
#define SPHERE_H_115209AE

#include "object.h"

#define PI 3.14159265359

class Sphere : public Object
{
public:
    Sphere(Point position,double r, double ang, Vector axis) : position(position), r(r), ang(ang), axis(axis)
        { }

    virtual Hit intersect(const Ray &ray);
    virtual Point mappingTexture(const Ray &ray, const double &min_hit);

    const Point position;
    const double r;
    const double ang;
    const Vector axis;
};

#endif /* end of include guard: SPHERE_H_115209AE */
