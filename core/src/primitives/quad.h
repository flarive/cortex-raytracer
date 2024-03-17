#ifndef QUAD_H
#define QUAD_H

#include "../constants.h"
#include "hittable.h"
#include "hittable_list.h"
#include "../misc/ray.h"

#include <glm/glm.hpp>
#include "../utilities/types.h"

#include <cmath>

/// <summary>
/// Quadrilateral (plane) primitive
/// </summary>
class quad : public hittable
{
public:
    quad(const point3& _Q, const vector3& _u, const vector3& _v, shared_ptr<material> m)
        : Q(_Q), u(_u), v(_v), mat(m)
    {
        auto n = glm::cross(u, v);
        normal = unit_vector(n);
        D = glm::dot(normal, Q);
        w = n / glm::dot(n, n);

        area = vector_length(n);// ????????????? .length();

        set_bounding_box();
    }

    void set_bounding_box()
    {
        bbox = aabb(Q, Q + u + v).pad();
    }

    aabb bounding_box() const override
    {
        return bbox;
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override
    {
        auto denom = glm::dot(normal, r.direction());

        // No hit if the ray is parallel to the plane.
        if (fabs(denom) < 1e-8)
            return false;

        // Return false if the hit point parameter t is outside the ray interval.
        auto t = (D - glm::dot(normal, r.origin())) / denom;
        if (!ray_t.contains(t))
            return false;

        // Determine the hit point lies within the planar shape using its plane coordinates.
        auto intersection = r.at(t);
        vector3 planar_hitpt_vector = intersection - Q;
        auto alpha = glm::dot(w, glm::cross(planar_hitpt_vector, v));
        auto beta = glm::dot(w, glm::cross(u, planar_hitpt_vector));

        if (!is_interior(alpha, beta, rec))
            return false;

        // Ray hits the 2D shape; set the rest of the hit record and return true.
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }

    bool is_interior(double a, double b, hit_record& rec) const
    {
        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.

        if ((a < 0) || (1 < a) || (b < 0) || (1 < b))
            return false;

        rec.u = a;
        rec.v = b;
        return true;
    }

    double pdf_value(const point3& origin, const vector3& v) const override
    {
        hit_record rec;

        if (!this->hit(ray(origin, v), interval(0.001, infinity), rec))
            return 0;

        auto distance_squared = rec.t * rec.t * vector_length_squared(v);
        auto cosine = fabs(dot(v, rec.normal) / vector_length(v));// .length()); ??????????????

        return distance_squared / (cosine * area);
    }

    vector3 random(const point3& origin) const override
    {
        auto p = Q + (random_double() * u) + (random_double() * v);
        return p - origin;
    }

    /// <summary>
    /// Update the internal AABB of the mesh.
    /// Warning: run this when the mesh is updated.
    /// </summary>
    void updateBoundingBox() override
    {
        // to implement
    }

private:
    point3 Q;
    vector3 u, v;
    shared_ptr<material> mat;
    aabb bbox;
    vector3 normal;
    double D;
    vector3 w;
    double area;
};


inline shared_ptr<hittable_list> box(const point3& a, const point3& b, shared_ptr<material> mat)
{
    // Returns the 3D box (six sides) that contains the two opposite vertices a & b.

    auto sides = make_shared<hittable_list>();

    // Construct the two opposite vertices with the minimum and maximum coordinates.
    auto min = point3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
    auto max = point3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));

    auto dx = vector3(max.x - min.x, 0, 0);
    auto dy = vector3(0, max.y - min.y, 0);
    auto dz = vector3(0, 0, max.z - min.z);

    sides->add(make_shared<quad>(point3(min.x, min.y, max.z), dx, dy, mat)); // front
    sides->add(make_shared<quad>(point3(max.x, min.y, max.z), -dz, dy, mat)); // right
    sides->add(make_shared<quad>(point3(max.x, min.y, min.z), -dx, dy, mat)); // back
    sides->add(make_shared<quad>(point3(min.x, min.y, min.z), dz, dy, mat)); // left
    sides->add(make_shared<quad>(point3(min.x, max.y, max.z), dx, -dz, mat)); // top
    sides->add(make_shared<quad>(point3(min.x, min.y, min.z), dx, dz, mat)); // bottom

    return sides;
}


#endif