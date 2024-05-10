#pragma once

#include "../aabb.h"
#include "../materials/material.h"
#include "../utilities/types.h"
#include "ray.h"

class hit_record
{
public:
	point3 hit_point; // point (coordinates) where the hit occurred
	vector3 normal; // normal vector where the hit occurred
	std::shared_ptr<material> mat; // material of the object hit by the ray
	double t;
	double u; // u mapping coordinate
	double v; // v mapping coordinate
	bool front_face; // front-face tracking (object was hit from outside (frontface) or inside (backface) ?)
	std::string name; // name of the object that was hit
	aabb bbox; // bounding box size of the object that was hit
	
	// to delete ? just test
	bool is_shadowed = false;

	vector3 tangent; // tangent vector calculated from the normal (obj models only)
	vector3 bitangent; // bitangent vector calculated from the normal (obj models only)

	void set_face_normal(const ray& r, const vector3& outward_normal);
};