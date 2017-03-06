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
    return (N + 1).normalized();
}

Color Scene::phongColor(Material *material, const Point &hit, const Vector &N, const Vector &V, Object *obj, size_t reflects)
{
    Color color;
    Hit shit = Hit::NO_HIT(); //shadow hit, I dunno man =D

    //ambient part
    color += material->color * material->ka;

    //for all lights.
    for(size_t i = 0; i < lights.size(); ++i)
    {
        Vector L = (lights[i]->position - hit).normalized();
        Vector R = (2 * L.dot(N) * N - L).normalized();

        if(shadows && collide(Ray(lights[i]->position, -L), &shit) != obj) continue;

        //diffuse part
        color += max(0.0, L.dot(N)) * material->color * lights[i]->color * material->kd;

        //specular part
        color += pow(max(0.0, R.dot(V)), material->n) * lights[i]->color * material->ks;
    }

    Vector R = (-V - 2 * -V.dot(N) * N);
    if(reflects != 0) color += trace(Ray(hit, R), reflects - 1) * material->ks;

    color.clamp();
    return color;
}

Scene::Scene()
{
    renderMode = PHONG;

    camera = false;
    shadows = false;
    depthOfField = false;

    supersampling = 0;
    reflectionDepth = 0;
    width = height = 400;
    apertureRadius = 0;
    apertureSamples = 0;
}

Object *Scene::collide(const Ray &ray, Hit *min_hit)
{
    (*min_hit) = Hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<(*min_hit).t) {
            *min_hit = hit;
            obj = objects[i];
        }
    }
    return obj; //returns null on no hit.
}

Color Scene::trace(const Ray &ray, size_t reflects)
{
    Hit min_hit = Hit::NO_HIT();
    Object *obj = collide(ray, &min_hit);

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Material *material = obj->material;            //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector

    Color color(0.0, 0.0, 0.0);

    switch(renderMode)
    {
        case PHONG:
            color = phongColor(material, hit, N, V, obj, reflects);
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

    //setup View
    Vector G, A, B, H(1, 0, 0), V(0, 1, 0);
    Point origin(0, 0, 0);

    int w = img.width();
    int h = img.height();
    double pixelSize = 1;


    if (camera) {
        pixelSize = up.length();
        G = (center - eye).normalized();
        A = (G.cross(up)).normalized();
        B = (A.cross(G)).normalized();

        //new basic unit vector
        H = pixelSize * A;
        V = pixelSize * B;
        origin = center - (w/2)*(H) - (h/2)*(V);
    }

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {

            //anti - aliasing
            double offset = (pixelSize) / supersampling;
            Color averageColor(0.0, 0.0, 0.0);
            Point pixel = origin + x*(H) + y*(V);

            if(depthOfField)
            {
                double c = apertureRadius / (up.length() * sqrt(apertureSamples));

                for(size_t dof = 0; dof < apertureSamples; ++dof)
                {
                    double r = c * sqrt(dof);
                    double theta = dof * GOLDEN_ANGLE;
                    Vector dofeye = eye;

                    dofeye = dofeye + (r * A * cos(theta)); //y displacement
                    dofeye = dofeye + (r * up * sin(theta)); //x displacement

                    //loop through points in one pixel
                    double i = pixel.x;
                    while (i < pixel.x + pixelSize) {
                        double j = pixel.y;
                        while (j < pixel.y + pixelSize) {
                            Point des(i + offset / 2, h - pixelSize - j + offset / 2, pixel.z);
                            Ray ray(dofeye, (des-dofeye).normalized());
                            Color col = trace(ray, reflectionDepth);
                            averageColor += col;
                            j += offset;
                        }
                        i += offset;
                    }
                }

                //get average color
                averageColor /= (supersampling * supersampling) * apertureSamples;
                img(x,y) = averageColor;
            }
            else
            {
                //loop through points in one pixel
                double i = pixel.x;
                while (i < pixel.x + pixelSize) {
                    double j = pixel.y;
                    while (j < pixel.y + pixelSize) {
                        Point des(i + offset / 2, h - pixelSize - j + offset / 2, pixel.z);
                        Ray ray(eye, (des-eye).normalized());
                        Color col = trace(ray, reflectionDepth);
                        averageColor += col;
                        j += offset;
                    }
                    i += offset;
                }

                //get average color
                averageColor /= (supersampling * supersampling);
                img(x,y) = averageColor;
            }
        }
    }

    if(renderMode == ZBUFFER)
    {
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
    camera = false;
}

void Scene::setCamera(Triple e, Triple c, Triple u)
{
    camera = true;
    eye = e;
    center = c;
    up = u;
}

void Scene::setShadows(bool s)
{
    shadows = s;
}

void Scene::setReflectionDepth(int d)
{
    reflectionDepth = d;
}

void Scene::setSupersampingFactor(int f)
{
    supersampling = f;
}

void Scene::setDepthOfField(int radius, int samples)
{
    depthOfField = true;
    apertureRadius = radius;
    apertureSamples = samples;
}

void Scene::setViewsize(int x, int y)
{
    width = x;
    height = y;
}

void Scene::printSettings()
{
    std::cout << "Scene with " << objects.size() << " objects.\n";
    std::cout << "    Lights: " << lights.size() << ".\n";
    std::cout << "    Shadows: " << (shadows ? "true" : "false") << ".\n";
    std::cout << "    Supersampling: " << supersampling << ".\n";
    std::cout << "    Reflection depth: " << reflectionDepth << ".\n";
    std::cout << "    Image dimensions: [" << width << ", " << height << "].\n";
    std::cout << "    Rendermode: ";
    if(renderMode == PHONG) std::cout << "Phong shading.\n";
    else if(renderMode == ZBUFFER) std::cout << "Depth render.\n";
    else if(renderMode == NORMAL) std::cout << "Normals.\n";
    else std::cout << "Unknown.\n";

    std::cout << "    Looking model: ";
    if(camera)
    {
        std::cout << "Camera model. \n";
        std::cout << "        Up: " << up << ".\n";
        std::cout << "        Eye: " << eye << ".\n";
        std::cout << "        Lookat: " << center << ".\n";
        if(depthOfField)
        {
            std::cout << "        Depth of field enabled.\n";
            std::cout << "        apertureRadius: " << apertureRadius << ".\n";
            std::cout << "        apertureSamples: " << apertureSamples << ".\n";
        } else std::cout << "        Depth of field disabled.\n";
    }
    else
    {
        std::cout << "Simple eye model.\n";
        std::cout << "        Eye: " << eye << ".\n";
    }
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
