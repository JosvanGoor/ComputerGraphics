#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include <vector>

#include "object.h"
#include "sphere.h"
#include "Triangle.hpp"

class Mesh : public Object
{

public:
    //Sphere bounding_sphere; //TODO: implement :)
    std::vector<Triangle> triangles;

    Mesh(const std::string &file);

    Hit intersect(const Ray &ray);

};

#endif 