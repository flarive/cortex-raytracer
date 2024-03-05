#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class vec3;
class material;

class sphere : public hittable
{
public:
    //sphere(point3 _center, double _radius)
    //    : center(_center), radius(_radius), mat(nullptr) {}
    //
    //sphere(point3 _center, double _radius, shared_ptr<material> _material)
    //    : center(_center), radius(_radius), mat(_material) {}


    // Stationary Sphere
    sphere(point3 _center, double _radius, shared_ptr<material> _material)
        : center1(_center), radius(_radius), mat(_material), is_moving(false)
    {
        // calculate stationary sphere bounding box for ray optimizations
        auto rvec = vec3(radius, radius, radius);
        bbox = aabb(center1 - rvec, center1 + rvec);
    }

    // Moving Sphere
    sphere(point3 _center1, point3 _center2, double _radius, shared_ptr<material> _material)
        : center1(_center1), radius(_radius), mat(_material), is_moving(true)
    {
        // calculate moving sphere bounding box for ray optimizations
        auto rvec = vec3(radius, radius, radius);
        aabb box1(_center1 - rvec, _center1 + rvec);
        aabb box2(_center2 - rvec, _center2 + rvec);
        bbox = aabb(box1, box2);

        
        center_vec = _center2 - _center1;
    }

    aabb bounding_box() const override
    {
        return bbox;
    }

    

    /// <summary>
    /// Logic of sphere ray hit detection
    /// </summary>
    /// <param name="r"></param>
    /// <param name="ray_t"></param>
    /// <param name="rec"></param>
    /// <returns></returns>
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override
    {
        point3 center = is_moving ? sphere_center(r.time()) : center1;
        vec3 oc = r.origin() - center;
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (-half_b + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        // number of hits encountered by the ray (only the nearest ?)
        rec.t = root;

        // point coordinate of the hit
        rec.p = r.at(rec.t);

        // material of the hit object
        rec.mat = mat;

        // set normal and front-face tracking
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);

        return true;
    }

private:
    point3 center1;
    double radius;
    shared_ptr<material> mat;
    bool is_moving;
    vec3 center_vec;
    aabb bbox; // bounding box

    point3 sphere_center(double time) const
    {
        // Linearly interpolate from center1 to center2 according to time, where t=0 yields
        // center1, and t=1 yields center2.
        return center1 + time * center_vec;
    }
};

#endif