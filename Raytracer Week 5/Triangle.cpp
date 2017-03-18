#include "Triangle.hpp"


Triangle::Triangle(Point v0, Point v1, Point v2, Vector n0, Vector n1, Vector n2)
    :   v0(v0), v1(v1), v2(v2), n0(n0), n1(n1), n2(n2) {}
        //edge0(v1 - v0), edge1(v2 - v0), edge2(v0 - v2), 
        //N(edge0.cross(edge1).normalized()) {}

//http://www.lighthouse3d.com/tutorials/maths/ray-triangle-intersection/

Hit Triangle::intersect(const Ray &ray)
{
    Vector e1 = v1 - v0;
    Vector e2 = v2 - v0;
    Vector e1e2 = e1.cross(e2).normalized();

    Vector p = ray.D.cross(e2);
    float a = e1.dot(p);
    if(a < 0.0001) return Hit::NO_HIT();

    float f =  1 / a;
    Vector s = ray.O - v0;
    float u = f*(s.dot(p));
    if(u < 0.0 || u > 1.0)
        return Hit::NO_HIT();

    Vector q = s.cross(e1);
    float v = f * (ray.D.dot(q));
    if(v < 0.0 || u + v > 1.0)
        return Hit::NO_HIT();

    float t = f * (e2.dot(q));

    Vector norm = (n0 + (u * (n1 - n0)) + (v * (n2 - n0)));

    //return Hit(t, e1e2, this);
    return Hit(t, norm.normalized(), this);
}

Color Triangle::colorAt(const Point &point)
{
    if(material->texture == NULL) return material->color;

    Vector u = v1 - v0;
    Vector v = v2 - v0;
    Vector w = point - v0;

    Vector vcw = v.cross(w);
    Vector vcu = v.cross(u);
    if(vcw.dot(vcu) < 0.0) return material->color;

    Vector ucw = u.cross(w);
    Vector ucv = u.cross(v);

    if(ucw.dot(ucv) < 0.0) return material->color;

    float denom = ucv.length();
    float r = vcw.length() / denom;
    float t = ucw.length() / denom;

    float tcw = 1.0 - r - t;
    float tcu = (t0.x * tcw) + (t1.x * r) + (t2.x * t);
    float tcv = (t0.y * tcw) + (t1.y * r) + (t2.y * t);

    if(tcu < 0 || tcu > 1.0 || tcv < 0 || tcv > 1.0)
    {
        std::cout << "denom: " << denom << "\n";
        std::cout << "r: " << r << "\n";
        std::cout << "t: " << t << "\n";
        std::cout << "tcw: " << tcw << "\n";
        std::cout << "tcu: " << tcu << "\n";
        std::cout << "tcv: " << tcw << "\n";
        std::cout << "t0: " << t0 << "\n";
        std::cout << "t1: " << t1 << "\n";
        std::cout << "t2: " << t2 << "\n";
    }

    return material->texture->colorAt(tcu, 1.0 - tcv);

/*
    Vector f1 = v0 - point;
    Vector f2 = v1 - point;
    Vector f3 = v2 - point;

    float a = (v0 - v1).cross(v0 - v2).length();
    float a1 = f2.cross(f3).length() / a;
    float a2 = f3.cross(f1).length() / a;
    float a3 = f1.cross(f2).length() / a;

    Vector uvz = (t0 * a1 + t1 * a2 + t2 * a3);
    if(uvz.x > 1.0 || uvz.x < 0.0) std::cout << "oh-ooh, u is: "  << uvz.x << std::endl;
    if(uvz.y > 1.0 || uvz.y < 0.0) std::cout << "oh-ooh, v is: "  << uvz.y << std::endl;

    return material->texture->colorAt(uvz.x, uvz.y);
*/
}