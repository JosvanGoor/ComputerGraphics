#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include <vector>
#include <limits>

#include "glm.h"
#include "object.h"
#include "sphere.h"
#include "Triangle.hpp"

class Mesh : public Object
{

public:
    Sphere *bounding_sphere;
    std::vector<Material*> materials;
    std::vector<Triangle> triangles;

    Mesh(const std::string &str, const Vector &pos, float scale, Material* defmat);
    ~Mesh();
    
    virtual Hit intersect(const Ray &ray);
    virtual Color colorAt(const Point &hit);

protected:
    void getMaterials(GLMmodel *model);
    void complexModel(GLMmodel *model, const Vector &pos);
    void simpleModel(GLMmodel *model, const Vector &pos);

};

#endif 