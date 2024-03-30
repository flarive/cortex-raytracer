#include "sphere_light.h"


aabb sphere_light::bounding_box() const
{
    return bbox;
}

point3 sphere_light::getPosition() const
{
    return center1;
}


bool sphere_light::hit(const ray& r, interval ray_t, hit_record& rec, int depth) const
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

double sphere_light::pdf_value(const point3& o, const vector3& v) const
{
    // This method only works for stationary spheres.
    hit_record rec;
    if (!this->hit(ray(o, v), interval(0.001, infinity), rec, 0))
        return 0;

    auto cos_theta_max = sqrt(1 - radius * radius / vector_length_squared(center1 - o));
    auto solid_angle = 2 * M_PI * (1 - cos_theta_max);

    return  1 / solid_angle;
}


vector3 sphere_light::random(const point3& o) const
{
    vector3 direction = center1 - o;
    auto distance_squared = vector_length_squared(direction);
    onb uvw;
    uvw.build_from_w(direction);
    return uvw.local(random_to_sphere(radius, distance_squared));
}