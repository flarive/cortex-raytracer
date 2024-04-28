#pragma once

#include "../primitives/hittable.h"
#include "../materials/material.h"
#include "types.h"


class ObjectFactory
{
public:
	ObjectFactory() = delete;

	static std::shared_ptr<hittable> createBox(const std::string name, const point3& p0, const point3& p1, const std::shared_ptr<material>& material);

	static std::shared_ptr<hittable> createCylinder(const point3& center, double radius, double height, const std::shared_ptr<material>& material);

	static std::shared_ptr<hittable> createSphere(const std::string name, const point3& center, double radius, const std::shared_ptr<material>& material);

	static std::shared_ptr<hittable> createCone(const point3& center, double height, double radius, const std::shared_ptr<material>& material);

	static std::shared_ptr<hittable> createPlane(const point3& p0, point3 p1, const std::shared_ptr<material>& material);

	static std::shared_ptr<hittable> createDirectionalLight(std::string name, const point3& pos, const vector3& u, const vector3& v, double intensity, color rgb, bool invisible);

	static std::shared_ptr<hittable> createOmniDirectionalLight(std::string name, const point3& pos, double radius, double intensity, color rgb, bool invisible);
};