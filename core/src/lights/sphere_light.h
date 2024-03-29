#pragma once

#include "light.h"
#include "../primitives/hittable.h"
#include "../misc/color.h"
#include "../materials/diffuse_light.h"
#include "../utilities/math_utils.h"
#include "../singleton.h"

/// <summary>
/// Sphere light
/// </summary>
class sphere_light : public light
{
public:
    sphere_light(point3 _center, double _radius, double _intensity, color _color, string _name = "SphereLight", bool _invisible = true)
    {
        intensity = _intensity;
        c = _color * _intensity;
        invisible = _invisible;

        center1 = _center;
        radius = _radius;

        name = _name;

        mat = std::make_shared<diffuse_light>(c, false, invisible);

        
        // calculate stationary sphere bounding box for ray optimizations
        vector3 rvec = vector3(radius, radius, radius);
        bbox = aabb(center1 - rvec, center1 + rvec);
    }

    aabb bounding_box() const override
    {
        return bbox;
    }

	point3 getPosition() const override
	{
        return center1;
	}

    /// <summary>
    /// Logic of sphere ray hit detection
    /// </summary>
    /// <param name="r"></param>
    /// <param name="ray_t"></param>
    /// <param name="rec"></param>
    /// <returns></returns>
    bool hit(const ray& r, interval ray_t, hit_record& rec, int depth) const override
    {
        point3 center = center1;
        vector3 oc = r.origin() - center;
        auto a = vector_length_squared(r.direction());
        auto half_b = glm::dot(oc, r.direction());
        auto c = vector_length_squared(oc) - radius * radius;

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

		Singleton* singleton = Singleton::getInstance();
		if (singleton)
		{
			auto renderParams = singleton->value();
			if (invisible && depth == renderParams.recursionMaxDepth)
			{
				return false;
			}
		}


        // number of hits encountered by the ray (only the nearest ?)
        rec.t = root;

        // point coordinate of the hit
        rec.hit_point = r.at(rec.t);

        // material of the hit object
        rec.mat = mat;

        // name of the primitive hit by the ray
        rec.name = name;
        rec.bbox = bbox;

        // set normal and front-face tracking
        vector3 outward_normal = (rec.hit_point - center) / radius;
        rec.set_face_normal(r, outward_normal);

        // UV coordinates
        get_sphere_uv(outward_normal, rec.u, rec.v);

        return true;
    }

    double pdf_value(const point3& o, const vector3& v) const override
    {
        // This method only works for stationary spheres.
        hit_record rec;
        if (!this->hit(ray(o, v), interval(0.001, infinity), rec, 0))
            return 0;

        auto cos_theta_max = sqrt(1 - radius * radius / vector_length_squared(center1 - o));
        auto solid_angle = 2 * M_PI * (1 - cos_theta_max);

        return  1 / solid_angle;
    }

    /// <summary>
    /// Random special implementation for sphere lights (override base)
    /// </summary>
    /// <param name="origin"></param>
    /// <returns></returns>
    vector3 random(const point3& o) const override
    {
        vector3 direction = center1 - o;
        auto distance_squared = vector_length_squared(direction);
        onb uvw;
        uvw.build_from_w(direction);
        return uvw.local(random_to_sphere(radius, distance_squared));
    }

private:
    point3 center1;
    double radius;
    vector3 center_vec;
};