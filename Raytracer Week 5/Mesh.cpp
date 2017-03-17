#include "Mesh.hpp"

#include "glm.h"

Mesh::Mesh(const std::string &str, const Vector &pos, float scale)
{
    GLMmodel *model = glmReadOBJ(str.c_str());
    glmUnitize(model);
    if(scale != 1.0f) glmScale(model, scale);

    //std::cout << "Reading mesh: \n";
    //std::cout << "numvertices: " << model->numvertices << "\n";
    //std::cout << "nummaterials: " << model->nummaterials << "\n";
    //std::cout << "numgroups: " << model->numgroups << "\n";

    triangles.reserve(model->numtriangles);

    for(size_t i = 0; i < model->numtriangles; ++i)
    {
        Point v0(
            model->vertices[3 * model->triangles[i].vindices[0]+0] + pos.x,
            model->vertices[3 * model->triangles[i].vindices[0]+1] + pos.y,
            model->vertices[3 * model->triangles[i].vindices[0]+2] + pos.z);

        Point v1(
            model->vertices[3 * model->triangles[i].vindices[1]+0] + pos.x,
            model->vertices[3 * model->triangles[i].vindices[1]+1] + pos.y,
            model->vertices[3 * model->triangles[i].vindices[1]+2] + pos.z);

        Point v2(
            model->vertices[3 * model->triangles[i].vindices[2]+0] + pos.x,
            model->vertices[3 * model->triangles[i].vindices[2]+1] + pos.y,
            model->vertices[3 * model->triangles[i].vindices[2]+2] + pos.z);

        Triangle tri(v0, v1, v2);
        tri.material = material;
        triangles.push_back(tri);

    }
    //1.74(sqrt(3)) because this is the max distance from the centre.
    bounding_sphere = new Sphere(pos, scale * 1.74);

    std::cout << "Mesh read: " << triangles.size() << " triangles!" << std::endl;
    glmDelete(model);
}

Mesh::~Mesh()
{
    delete bounding_sphere;
}

Hit Mesh::intersect(const Ray &ray)
{
    
    Hit min_hit = bounding_sphere->intersect(ray);
    if(!min_hit.hit)
    {
        return Hit::NO_HIT();
    }


    min_hit = Hit(std::numeric_limits<double>::infinity(), Vector());

    for(size_t i = 0; i < triangles.size(); ++i)
    {
        Hit hit = triangles[i].intersect(ray);
        if(hit.t < min_hit.t)
        {
            min_hit = hit;
        }
    }

    return min_hit;
}

Color Mesh::colorAt(const Point &point)
{
    return material->color;
}