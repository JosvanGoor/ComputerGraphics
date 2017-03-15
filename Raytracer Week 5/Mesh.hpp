#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include <vector>
#include <limits>

#include "object.h"
#include "sphere.h"
#include "Triangle.hpp"

class Mesh : public Object
{

public:
    Sphere *bounding_sphere;
    std::vector<Triangle> triangles;

    Mesh(const std::string &str, const Vector &pos, float scale);
    ~Mesh();
    
    Hit intersect(const Ray &ray);

};

#endif 