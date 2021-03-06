//
//  Framework for a raytracer
//  File: sphere.cpp
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

#include "sphere.h"
#include <iostream>
#include <math.h>

/************************** Sphere **********************************/

Hit Sphere::intersect(const Ray &ray)
{
    /****************************************************
     * RT1.1: INTERSECTION CALCULATION
     *
     * Given: ray, position, r
     * Sought: intersects? if true: *t
     *
     * Insert calculation of ray/sphere intersection here.
     *
     * You have the sphere's center (C) and radius (r) as well as
     * the ray's origin (ray.O) and direction (ray.D).
     *
     * If the ray does not intersect the sphere, return false.
     * Otherwise, return true and place the distance of the
     * intersection point from the ray origin in *t (see example).
     ****************************************************/

    // place holder for actual intersection calculation

    Vector o = ray.O;
    Vector d = ray.D;
    Vector oc = o - position;


    //Solve equation At^2 + Bt + C = 0
    double a = d.dot(d);
    double b = 2 * oc.dot(d);
    double c = oc.dot(oc) - r * r;
    double disc = b * b - 4 * a * c;
    double t;

    if (disc < 0) return Hit::NO_HIT();
    else {
        disc = sqrt(disc);
        double t1 = (-b - disc) / (2 * a);
        double t2 = (-b + disc) / (2 * a);

        //Choose the nearest point
        if(t1 < 0) t = t2;
        else if(t2 < 0) t = t1;
        else if (t1 < t2) t = t1;
        else t = t2;
    }

    //not 0, this also prevents "surface acne"
    if(t < 0.0001) return Hit::NO_HIT();

    /****************************************************
     * RT1.2: NORMAL CALCULATION
     *
     * Given: t, C, r
     * Sought: N
     *
     * Insert calculation of the sphere's normal at the intersection point.
     ****************************************************/

    Vector intersect = ray.at(t);
    Vector N = (intersect - position) / r;
    if(ray.D.dot(N) > 0) N = -N; //inside the sphere

    return Hit(t,N, this);
}

Color Sphere::colorAt(const Point &hit)
{
    if(material->texture == NULL) return material->color;

    //std::cout << "found tex under pointer to " << (long)(material->texture) << std::endl;

    double angR = ang * PI / (double)180;
    Vector u = axis.normalized();
    Point hitPoint = hit; 
    hitPoint-= position;

    Point newPoint;
    newPoint.x = hitPoint.x*(cos(angR) + u.x*u.x*(1-cos(angR))) + hitPoint.y*(u.x*u.y*(1-cos(angR)) - u.z*sin(angR)) + hitPoint.z*(u.x*u.z*(1-cos(angR)) + u.y*sin(angR));
    newPoint.y = hitPoint.x*(u.y*u.x*(1-cos(angR)) + u.z*sin(angR)) + hitPoint.y*(cos(angR)+u.y*u.y*(1-cos(angR))) + hitPoint.z*(u.y*u.z*(1-cos(angR))-u.x*sin(angR));
    newPoint.z = hitPoint.x*(u.z*u.x*(1-cos(angR))-u.y*sin(angR)) + hitPoint.y*(u.z*u.y*(1-cos(angR))+u.x*sin(angR)) + hitPoint.z*(cos(angR)+u.z*u.z*(1-cos(angR)));
    newPoint += position;

    double theta = acos((newPoint.z - position.z) / r);
    double phi = atan2(newPoint.y - position.y, newPoint.x - position.x) + PI;

    double uu = phi / (2 * PI);
    double vv = theta / PI;

    return material->texture->colorAt(uu, vv);
}
