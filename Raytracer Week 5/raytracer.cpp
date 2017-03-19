//
//  Framework for a raytracer
//  File: raytracer.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Author: Maarten Everts
//
//  This framework is inspired by and uses code of the raytracer framework of
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html
//

#include "raytracer.h"
#include "object.h"
#include "material.h"
#include "light.h"
#include "image.h"
#include "yaml/yaml.h"
#include <ctype.h>
#include <fstream>
#include <assert.h>

#include "sphere.h"
#include "Disk.h"
#include "Mesh.hpp"
#include "Triangle.hpp"
#include "Cylinder.h"

// Functions to ease reading from YAML input
void operator >> (const YAML::Node& node, Triple& t);
Triple parseTriple(const YAML::Node& node);

void operator >> (const YAML::Node& node, Triple& t)
{
    assert(node.size()==3);
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;
}

Triple parseTriple(const YAML::Node& node)
{
    Triple t;
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;
    return t;
}

Material* Raytracer::parseMaterial(const YAML::Node& node)
{
    Material *m = new Material();

    if (node.FindValue("texture")) {
        std::string text;
        node["texture"] >> text;
        const char *c = text.c_str();

        m->texture = new Image(c);
    }else m->texture = NULL;

    node["color"] >> m->color;
    node["ka"] >> m->ka;
    node["kd"] >> m->kd;
    node["ks"] >> m->ks; 
    node["n"] >> m->n;
    return m;
}

Object* Raytracer::parseObject(const YAML::Node& node)
{
    Object *returnObject = NULL;
    std::string objectType;
    node["type"] >> objectType;

    if (objectType == "sphere")
    {
        Point pos;
        node["position"] >> pos;

        double r;
        Vector axis;        
        if (node["radius"].size() <= 1) {
            node["radius"] >> r;       
        }
        else {   
            node["radius"][0] >> r;        
            axis = parseTriple(node["radius"][1]);
        }

        double ang = 0.0;
        if (node.FindValue("angle")) node["angle"] >> ang;    

        Sphere *sphere = new Sphere(pos,r, ang, axis);
        returnObject = sphere;
    }
    else if(objectType == "disk")
    {
        Point pos;
        node["position"] >> pos;
        Vector N;
        node["normal"] >> N;
        double r;
        node["radius"] >> r;
        Disk *disk = new Disk(pos, N, r);
        returnObject = disk;
    }
    else if(objectType == "cylinder")
    {
        Point start;
        node["position"] >> start;
        Vector dir;
        node["direction"] >> dir;
        double r, l;
        node["radius"] >> r;
        node["length"] >> l;

        Cylinder *cyl = new Cylinder(start, dir, r, l);
        returnObject = cyl;
    }
    else if(objectType == "mesh")
    {
        std::string file;
        node["file"] >> file;
        Vector pos;
        node["position"] >> pos;
        float scale;
        node["scale"] >> scale;
        
        Mesh *mesh = new Mesh(file, pos, scale, parseMaterial(node["material"]));
        return mesh;
    }

    if (returnObject) {
        // read the material and attach to object
        returnObject->material = parseMaterial(node["material"]);
    }

    return returnObject;
}

Light* Raytracer::parseLight(const YAML::Node& node)
{
    Point position;
    node["position"] >> position;
    Color color;
    node["color"] >> color;
    return new Light(position,color);
}

void Raytracer::parseCamera(const YAML::Node &node)
{
    scene->setCamera(parseTriple(node["eye"]),
                     parseTriple(node["center"]),
                     parseTriple(node["up"]));
    parseSize(node["viewSize"]);
    if(node.FindValue("apertureSamples"))
    {
        scene->setDepthOfField(node["apertureRadius"], node["apertureSamples"]);
    }
}

void Raytracer::parseSize(const YAML::Node &node)
{
    width = node[0];
    height = node[1];
    scene->setViewsize(width, height);
}

void Raytracer::parseGoochParameters(const YAML::Node &node) {
    double b, y, alpha, beta;
    node["b"] >> b;
    node["y"] >> y;
    node["alpha"] >> alpha;
    node["beta"] >> beta;
    scene->setGoochParameters(b, y, alpha, beta);
}

/*
* Read a scene from file
*/

bool Raytracer::readScene(const std::string& inputFilename)
{
    // Initialize a new scene
    scene = new Scene();

    // Open file stream for reading and have the YAML module parse it
    std::ifstream fin(inputFilename.c_str());
    if (!fin) {
        cerr << "Error: unable to open " << inputFilename << " for reading." << endl;;
        return false;
    }
    try {
        YAML::Parser parser(fin);
        if (parser) {
            YAML::Node doc;
            parser.GetNextDocument(doc);

            // Find the rendermode parameter if its available
            doc.FindValue("RenderMode") ? scene->setRenderMode(doc["RenderMode"]) : scene->setRenderMode("phong");
            doc.FindValue("Shadows") ? scene->setShadows(doc["Shadows"]) : scene->setShadows(false);
            doc.FindValue("MaxRecursionDepth") ? scene->setReflectionDepth(doc["MaxRecursionDepth"]) : scene->setReflectionDepth(0);
            doc.FindValue("SuperSampling") ? scene->setSupersampingFactor(doc["SuperSampling"]["factor"]) : scene->setSupersampingFactor(1);

            if (doc.FindValue("GoochParameters")) {
                parseGoochParameters(doc["GoochParameters"]);
            }


            // Read scene configuration options
            if(doc.FindValue("Camera"))
            {
                parseCamera(doc["Camera"]);
            }
            else
            {
                scene->setEye(parseTriple(doc["Eye"]));
            }

            // Read and parse the scene objects
            const YAML::Node& sceneObjects = doc["Objects"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of objects." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneObjects.begin();it!=sceneObjects.end();++it) {
                Object *obj = parseObject(*it);
                // Only add object if it is recognized
                if (obj) {
                    scene->addObject(obj);
                } else {
                    cerr << "Warning: found object of unknown type, ignored." << endl;
                }
            }

            // Read and parse light definitions
            const YAML::Node& sceneLights = doc["Lights"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of lights." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneLights.begin();it!=sceneLights.end();++it) {
                scene->addLight(parseLight(*it));
            }
        }
        if (parser) {
            cerr << "Warning: unexpected YAML document, ignored." << endl;
        }
    } catch(YAML::ParserException& e) {
        std::cerr << "Error at line " << e.mark.line + 1 << ", col " << e.mark.column + 1 << ": " << e.msg << std::endl;
        return false;
    }

    cout << "YAML parsing results: " << scene->getNumObjects() << " objects read." << endl;
    return true;
}

void Raytracer::renderToFile(const std::string& outputFilename)
{
    Image img(width, height);
    cout << "Tracing... ";
    scene->printSettings();
    scene->render(img);
    cout << "Writing image to " << outputFilename << "..." << endl;
    img.write_png(outputFilename.c_str());
    cout << "Done." << endl;

    delete scene;
}
