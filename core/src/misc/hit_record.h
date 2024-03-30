#pragma once

#include "../aabb.h"
#include "../materials/material.h"
#include "../utilities/types.h"
#include "ray.h"

#include <glm/glm.hpp>


//class material;
//class aabb;

class hit_record
{
public:
	point3 hit_point; // point (coordinates) where the hit occurred
	vector3 normal; // normal vector where the hit occurred
	std::shared_ptr<material> mat; // material of the object hit by the ray
	double t;
	double u;
	double v;
	bool front_face; // front-face tracking (object was hit from outside (frontface) or inside (backface) ?)
	std::string name; // name of the object that was hit
	aabb bbox; // bounding box size of the object that was hit

	void set_face_normal(const ray& r, const vector3& outward_normal);
};