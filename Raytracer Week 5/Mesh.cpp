#include "Mesh.hpp"

#include "glm.h"

Mesh::Mesh(const std::string &str)
{
    GLMmodel *model = glmReadOBJ(str.c_str());
    glmUnitize(model);

    for(size_t i = 0; i < model->numtriangles; ++i)
    {
        //add triangles :)
    }

}

Hit Mesh::intersect(const Ray &ray)
{
    return Hit::NO_HIT();
}