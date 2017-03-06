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
#include <sstream>
#include "triple.h"
#include "light.h"
#include "object.h"
#include "image.h"

#define GOLDEN_ANGLE (180*(3-sqrt(5)))

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
    Triple center;
    Triple up;
    size_t width;
    size_t height;


    double distMin;
    double distMax;

    bool camera; //if true use complex camera, otherwise simple eye model.
    bool shadows;
    bool depthOfField;
    RenderMode renderMode;
    size_t supersampling;
    size_t reflectionDepth;
    size_t apertureRadius;
    size_t apertureSamples;

    //colors according to the distance from camera.
    void finalizeDepthRender(Image &img); //finalizes rendering (depth needs min and max).
    Color depthColor(double distance); //

    //colors the colors based on vector-normal
    Color normalColor(const Vector &N);
    //colors using the phong lighting model
    Color phongColor(Material *material, const Point &hit, const Vector &N, const Vector &V, Object *obj, size_t reflects);

public:

    Scene(); //default constructor

    Object *collide(const Ray &ray, Hit *hit);
    Color trace(const Ray &ray, size_t reflects = 0);
    void render(Image &img);

    void addObject(Object *o);
    void addLight(Light *l);
    void setEye(Triple e);
    void setCamera(Triple eye, Triple center, Triple up);
    void setViewsize(int x, int y);
    void setRenderMode(std::string name);
    void setShadows(bool s);
    void setReflectionDepth(int d);
    void setSupersampingFactor(int f);
    void setDepthOfField(int radius, int samples);
    unsigned int getNumObjects() { return objects.size(); }
    unsigned int getNumLights() { return lights.size(); }

    void printSettings(); //outputs scene settings.
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
