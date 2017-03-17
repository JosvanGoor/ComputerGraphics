#include "Mesh.hpp"

Mesh::Mesh(const std::string &str, const Vector &pos, float scale, Material *defaultmat)
{
    this->material = defaultmat;

    GLMmodel *model = glmReadOBJ(str.c_str());
    //glmWeld(model, 0.00001);
    glmUnitize(model);
    glmFacetNormals(model);
    glmVertexNormals(model, 90);
    if(scale != 1.0f) glmScale(model, scale);

    std::cout << "Reading mesh: \n";
    std::cout << "numvertices: " << model->numvertices << "\n";
    std::cout << "nummaterials: " << model->nummaterials << "\n";
    std::cout << "numgroups: " << model->numgroups << "\n";
    triangles.reserve(model->numtriangles);
    
    if(model->numgroups > 1) 
        complexModel(model, pos);
    else simpleModel(model, pos);
    //simpleModel(model, pos);
    
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
    if(min_hit.object == bounding_sphere)
    {
        return Hit::NO_HIT();
    }
    std::cout << "yay hit the thing" << std::endl;

    min_hit = Hit(std::numeric_limits<double>::infinity(), Vector(), NULL);

    for(size_t i = 0; i < triangles.size(); ++i)
    {
        Hit hit = triangles[i].intersect(ray);
        if(hit.t < min_hit.t)
        {
            min_hit = hit;
        }
    }

    //min_hit.object = this;
    return min_hit;
}

Color Mesh::colorAt(const Point &point)
{
    return material->color;
}

void Mesh::getMaterials(GLMmodel *model)
{
    materials.reserve(model->nummaterials);
    for(size_t i = 0; i < model->nummaterials; ++i)
    {
        Material *material = new Material();
        GLMmaterial *toparse = &model->materials[i];

        material->color = Color(toparse->diffuse[0], toparse->diffuse[1], toparse->diffuse[2]);

        material->kd = (toparse->diffuse[0] 
                    + toparse->diffuse[1]
                    + toparse->diffuse[2]) / 3; //average rgb components or whatever.


        material->ka = (toparse->ambient[0] 
                    + toparse->ambient[1]
                    + toparse->ambient[2]) / 3; //average rgb components or whatever.

        material->ks = (toparse->specular[0] 
                    + toparse->specular[1]
                    + toparse->specular[2]) / 3; //average rgb components or whatever.

        material->n = toparse->shininess;

        materials.push_back(material);
    }
}

void Mesh::complexModel(GLMmodel *model, const Vector &pos)
{
    getMaterials(model);

    GLMgroup *group = model->groups;
    while(group != NULL)
    {
        for(size_t i = 0; i < group->numtriangles; ++i)
        {
            Point v0(
                model->vertices[3 * model->triangles[group->triangles[i]].vindices[0]+0] + pos.y,
                model->vertices[3 * model->triangles[group->triangles[i]].vindices[0]+1] + pos.x,
                model->vertices[3 * model->triangles[group->triangles[i]].vindices[0]+2] + pos.z);

            Point v1(
                model->vertices[3 * model->triangles[group->triangles[i]].vindices[1]+0] + pos.x,
                model->vertices[3 * model->triangles[group->triangles[i]].vindices[1]+1] + pos.y,
                model->vertices[3 * model->triangles[group->triangles[i]].vindices[1]+2] + pos.z);

            Point v2(
                model->vertices[3 * model->triangles[group->triangles[i]].vindices[2]+0] + pos.x,
                model->vertices[3 * model->triangles[group->triangles[i]].vindices[2]+1] + pos.y,
                model->vertices[3 * model->triangles[group->triangles[i]].vindices[2]+2] + pos.z);

            Vector n0(
                model->normals[3 * model->triangles[group->triangles[i]].nindices[0]+0],
                model->normals[3 * model->triangles[group->triangles[i]].nindices[0]+1],
                model->normals[3 * model->triangles[group->triangles[i]].nindices[0]+2]);

            Vector n1(
                model->normals[3 * model->triangles[group->triangles[i]].nindices[1]+0],
                model->normals[3 * model->triangles[group->triangles[i]].nindices[1]+1],
                model->normals[3 * model->triangles[group->triangles[i]].nindices[1]+2]);

            Vector n2(
                model->normals[3 * model->triangles[group->triangles[i]].nindices[1]+0],
                model->normals[3 * model->triangles[group->triangles[i]].nindices[1]+1],
                model->normals[3 * model->triangles[group->triangles[i]].nindices[1]+2]);
                
            Triangle tri(v0, v1, v2, n0, n1, n2);
            tri.material = materials[group->material];
            triangles.push_back(tri);
        }
        group = group->next;
    }
}

void Mesh::simpleModel(GLMmodel *model, const Vector &pos)
{
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

        Vector n0(
            model->normals[3 * model->triangles[i].nindices[0]+0],
            model->normals[3 * model->triangles[i].nindices[0]+1],
            model->normals[3 * model->triangles[i].nindices[0]+2]);

        Vector n1(
            model->normals[3 * model->triangles[i].nindices[1]+0],
            model->normals[3 * model->triangles[i].nindices[1]+1],
            model->normals[3 * model->triangles[i].nindices[1]+2]);

        Vector n2(
            model->normals[3 * model->triangles[i].nindices[2]+0],
            model->normals[3 * model->triangles[i].nindices[2]+1],
            model->normals[3 * model->triangles[i].nindices[2]+2]);

        Triangle tri(v0, v1, v2, n0, n1, n2);
        tri.material = material;
        triangles.push_back(tri);
    }
}