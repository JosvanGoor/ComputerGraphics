//
//  Framework for a raytracer
//  File: scene.cpp
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

#include "scene.h"
#include "material.h"

#include <iostream>

void Scene::finalizeDepthRender(Image &img)
{
    for(int y = 0; y < img.height(); ++y)
    {
        for(int x = 0; x < img.width(); ++x)
        {
            double distance = img(x, y).r;
            if(distance == 0) continue;
            
            double diff = distMax - distMin;
            distance = distance - distMin;
            double value = 1 - distance / diff;
            
            Color c = Color(value, value, value);
            img.put_pixel(x, y, c);
        }
    }
}

Color Scene::depthColor(double distance)
{
    //store distance in Color.r, 
    //the color will be finalized to a greyscale when the depthrender is finalized.
    this->distMin = std::min(distance, distMin);
    this->distMax = std::max(distance, distMax);
    
    return Color(distance);
}

double max(double a, double b) {
    if (a > b) return a; else return b;
}
    
Color Scene::normalColor(const Vector &N)
{
    return (-N + 1).normalized();
}

Color Scene::phongColor(Material *material, const Point &hit, const Vector &N, const Vector &V)
{
    Color color;
    
    //for all lights.
    for(size_t i = 0; i < lights.size(); ++i)
    {   
        Vector L = (hit - lights[i]->position);
        L.normalize();
        
        Vector R = L - 2 * L.dot(N) * N;
        R.normalize();
        
        //ambient part
        color += material->color * material->ka;
        
        //diffuse part
        color += max(0.0, L.dot(N)) * material->color * lights[i]->color * material->kd;
        
        //specular part
        color += pow(max(0.0, R.dot(V)), material->n) * lights[i]->color * material->ks;
    }
   
    color.clamp();
    return color;
}

Scene::Scene()
{
    renderMode = PHONG;
}

Color Scene::trace(const Ray &ray)
{
    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<min_hit.t) {
            min_hit = hit;
            obj = objects[i];
        }
    }

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Material *material = obj->material;            //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector


    /****************************************************
    * This is where you should insert the color
    * calculation (Phong model).
    *
    * Given: material, hit, N, V, lights[]
    * Sought: color
    *
    * Hints: (see triple.h)
    *        Triple.dot(Vector) dot product
    *        Vector+Vector      vector sum
    *        Vector-Vector      vector difference
    *        Point-Point        yields vector
    *        Vector.normalize() normalizes vector, returns length
    *        double*Color        scales each color component (r,g,b)
    *        Color*Color        dito
    *        pow(a,b)           a to the power of b
    ****************************************************/
    
    Color color(0.0, 0.0, 0.0);
    
    switch(renderMode)
    {
        case PHONG:
            color = phongColor(material, hit, N, V);
            break;
        case ZBUFFER:
            color = depthColor(min_hit.t);
            break;
        case NORMAL:
            color = normalColor(N);
            break;
    }

    return color;
}

void Scene::render(Image &img)
{
    this->distMin = std::numeric_limits<double>::infinity();
    this->distMax = 0;
    
    int w = img.width();
    int h = img.height();
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Point pixel(x+0.5, h-1-y+0.5, 0);
            Ray ray(eye, (pixel-eye).normalized());
            Color col = trace(ray);
            img(x,y) = col;
        }
    }
    
    if(renderMode == ZBUFFER)
    {
        //If the values hit the limit rounding errors occur.
        //distMin *= 0.8;
        //distMax *= 1.2;
        
        finalizeDepthRender(img);
    }
}

void Scene::addObject(Object *o)
{
    objects.push_back(o);
}

void Scene::addLight(Light *l)
{
    lights.push_back(l);
}

void Scene::setEye(Triple e)
{
    eye = e;
}

void Scene::setRenderMode(std::string name)
{
    if(name == "phong") renderMode = PHONG;
    else if(name == "zbuffer") renderMode = ZBUFFER;
    else if(name == "normal") renderMode = NORMAL;
    else
    {
        std::cout << "Did not recognize rendermode \"" << name << "\", defaulting to phong" << std::endl;
        renderMode = PHONG;
    }
}
