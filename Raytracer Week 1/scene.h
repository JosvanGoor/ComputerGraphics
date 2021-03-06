//
//  Framework for a raytracer
//  File: scene.h
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

#ifndef SCENE_H_KNBLQLP6
#define SCENE_H_KNBLQLP6

#include <vector>
#include <limits>
#include <string>
#include "triple.h"
#include "light.h"
#include "object.h"
#include "image.h"

class Scene
{
private:
    
    enum RenderMode
    {
        PHONG,
        ZBUFFER,
        NORMAL
    };
    
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Triple eye;
    
    double distMin;
    double distMax;
    RenderMode renderMode;
    
    //colors according to the distance from camera.
    void finalizeDepthRender(Image &img); //finalizes rendering (depth needs min and max).
    Color depthColor(double distance); //
    
    //colors the colors based on vector-normal
    Color normalColor(const Vector &N);
    //colors using the phong lighting model
    Color phongColor(Material *material, const Point &hit, const Vector &N, const Vector &V);
    
public:
    
    Scene(); //default constructor
    
    Color trace(const Ray &ray);
    void render(Image &img);
    
    void addObject(Object *o);
    void addLight(Light *l);
    void setEye(Triple e);
    void setRenderMode(std::string name);
    unsigned int getNumObjects() { return objects.size(); }
    unsigned int getNumLights() { return lights.size(); }
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
